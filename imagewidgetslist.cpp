/** @file imagewidgetslist.cpp
  * Soubor s tridou ImageWidgetsList dedici ze trid QObject a QList<ImageInfo *>
  * obsahujici seznam widgetu s fotografiemi
  */

#include "imagewidgetslist.h"

ImageWidgetsList::ImageWidgetsList(QObject *parent) :
    QObject(parent)
{
    //selectedCount = 0;
}

void ImageWidgetsList::addImage(ImageInfo *imageWidget) {
    //idImageCount++;
    /*   if (imageWidget->loadFile(fileName))
    {
        return 1;
    }
*/

    this->append(imageWidget);

    connect( imageWidget, SIGNAL(mapAddMarker(int,double,double)), map, SLOT(addMarker(int,double,double)));
    connect( imageWidget, SIGNAL(mapAddObjectMarker(int,double,double)), map, SLOT(addObjectMarker(int,double,double)));

    connect( imageWidget, SIGNAL(deleteMarker(int)), map, SLOT(deleteMarker(int)));
    connect( imageWidget, SIGNAL(selected(int, bool)), map, SLOT(markerSelected(int,bool)));

    // connect( picturesFrame, SIGNAL(rubberBandRect(QRect,bool, bool)), imageWidget, SLOT(testRect(QRect, bool, bool)));
    // connect( imageWidget, SIGNAL(getFocus(int, int)), ui->scrollAreaPictures, SLOT(setImageFocus(int, int)));
    ///connect( map, SIGNAL(mClicked(int)), imageWidget, SLOT(checkMarkerClickedId(int)));
    connect( map, SIGNAL(setGpsInImage(int, double, double, double)),
             imageWidget, SLOT(setGpsFromMap(int, double,double, double)));

    connect(imageWidget->saveGpsAction, SIGNAL(triggered()),this,SLOT(saveExifGpsInSelected()) );
    connect(imageWidget->saveDateTimeAction, SIGNAL(triggered()),this,SLOT(saveExifDateTimeInSelected()) );
}

void ImageWidgetsList::deleteSelected() {
    if(this->isEmpty())
        return;


    bool isUnsaved = 0;
    //zjisti, jestli vybrane widgety nemaji neulozena data
    for(int i=this->length()-1; i>=0; i--) {
        ImageInfo *imageWidget= this->at(i);
        if(imageWidget->isClicked && !(imageWidget->imageData->isGpsSaved && imageWidget->imageData->isDateTimeSaved)){
            isUnsaved = 1;
            break;
        }
    }
    bool save = 0;
    if(isUnsaved) {
        QMessageBox *mb = new QMessageBox(QMessageBox::Question, tr("Save changes"),
                                          tr("Some of the pictures you are trying to close have EXIF metadata that weren't saved. "
                                             "Do you wish to save them now?"),
                                          QMessageBox::Cancel | QMessageBox::No  | QMessageBox::Yes);

        QPalette pal = mb->palette();
        pal.setColor(QPalette::Window, "#D0D0E7");
        mb->setPalette(pal);
        int ret = mb->exec();
        if (ret == QMessageBox::Yes) {
            save = 1;
        } else if(ret == QMessageBox::No) {
            save = 0;
        } else {
            return;
        }
    }
    //smaže vybrane widgety
    for(int i=this->length()-1; i>=0; i--) {
        ImageInfo *imageWidget= this->at(i);
        if(imageWidget->isClicked){
            if(save && !(imageWidget->imageData->isGpsSaved && imageWidget->imageData->isDateTimeSaved)) {
                imageWidget->saveDateTime();
                imageWidget->saveGps();
            }
            delete imageWidget;
            this->removeOne(imageWidget);
        }
    }
    emit showImages();
}

QList<int> ImageWidgetsList::selectedIdList() {
    QList<int> idList;
    for(int i=0; i<this->length(); i++) {
        if(this->at(i)->isClicked) {
            idList.append(this->at(i)->imageData->id);
        }
    }
    return idList;
}
void ImageWidgetsList::saveExifGpsInSelected() {
    for (int i=0; i<this->length(); i++) {
        if (this->at(i)->isClicked && !this->at(i)->imageData->isGpsSaved) {
            this->at(i)->saveGps();
        }
    }
}
void ImageWidgetsList::saveExifDateTimeInSelected() {
    for(int i=0; i<this->length(); i++) {
        if(this->at(i)->isClicked  && !this->at(i)->imageData->isDateTimeSaved) {
            this->at(i)->saveDateTime();
        }
    }
}
void ImageWidgetsList::saveExifGpsInAll() {
    qDebug() << "saveExifGPSInAll";
    for(int i=0; i<this->length(); i++) {
        if(!this->at(i)->imageData->isGpsSaved) {
            qDebug() << "saveExifGPSIn" << i;
            this->at(i)->saveGps();
        }
    }
}
void ImageWidgetsList::saveExifDateTimeInAll()
{
    for (int i=0; i<this->length(); i++) {
        if (!this->at(i)->imageData->isDateTimeSaved) {
            this->at(i)->saveDateTime();
        }
    }
}
void ImageWidgetsList::testRightClick(QPoint p) {
    for(int i = 0; i < this->length(); i++) {
        if(QRect(this->at(i)->pos(),this->at(i)->rect().size()).intersects(QRect(p,QSize(1,1)))) {
            //qDebug() << "ooooooo";
            if(this->at(i)->isClicked) {
                return;
            } else {
                selectOne(this->at(i)->imageData->id, 1, 0);
                return;
            }
        }
    }
    selectOne(-1, 1, 0);
    return;
}

void ImageWidgetsList::selectOne(int id, bool clickMarker, bool focus) {
    qDebug() << "selectOne" << id << clickMarker << focus;

    bool isOnly = 1;
    int iSelected = -1;
    for(int i = 0; i < this->length(); i++) {
        if (id == at(i)->imageData->id) {
            iSelected = i;
            if (clickMarker) {
                at(i)->select();
            } else {
                at(i)->click(focus);
            }

        } else if(!(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            at(i)->unselect();
        } else {
            if(at(i)->isClicked) {
                isOnly = 0;
            }
        }
    }
    if (iSelected == -1) {
        return;
    }
    if (isOnly) {
        QStringList exifList = at(iSelected)->exifInformation();
        if(!exifList.isEmpty()) {
            emit setExifInfo(exifList);
        }
    }
    //selectedCount = 1;
}

void ImageWidgetsList::rubberBandSelection(QRect r, bool apply) {
    int selectedCount = 0;
    int lastISelected = 0;
    for(int i=0; i<length(); i++) {
        QRect imageRect = QRect(at(i)->pos(),at(i)->rect().size());
        if(r.intersects(imageRect)) {
            if((QApplication::keyboardModifiers() & Qt::ControlModifier ) && at(i)->isClickedOrig) {//invertuju vyber

                if(apply) {
                    at(i)->unselect();
                } else {
                    at(i)->unclick();
                }

            } else {   //vyberu
                if(apply) {
                    at(i)->select();
                } else {
                    at(i)->click(0);
                }
                lastISelected = i;
            }

        } else if(!(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
            if(apply) {
                at(i)->unselect();
            } else {
                at(i)->unclick();
            }
        } else {
            at(i)->isClicked = at(i)->isClickedOrig;
            if(at(i)->isClicked) {
                if(apply) {
                    at(i)->select();
                } else {
                    at(i)->click(0);
                }
                lastISelected = i;
            } else {
                if(apply) {
                    at(i)->unselect();
                } else {
                    at(i)->unclick();
                }
            }
        }
        if(at(i)->isClicked) {
            selectedCount++;
        }
    }
    if(apply && selectedCount == 1) {
        emit setExifInfo(at(lastISelected)->exifInformation());
    } else if(apply && selectedCount == 0) {
        QStringList list;
        emit setExifInfo(list);
    }
}

bool ImageWidgetsList::isNotSaved() {
    for(int i=0; i<this->length(); i++) {
        if(!this->at(i)->imageData->isGpsSaved || !this->at(i)->imageData->isDateTimeSaved) {
            return true;
        }
    }
    return false;
}

/*void ImageWidgetsList::changeLabelsVisibility(QAction *a)
{
    qDebug() << a->isChecked() << a->data();
}*/
