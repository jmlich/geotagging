/** @file imagedinfo.cpp
  * Soubor s trifou ImageInfo dedici ze tridy QWidget zobrazujici fotografii a popisky
  */

#include <QDesktopServices>
#include <QAction>
#include <QDebug>
#include "imageinfo.h"
#include "ui_imageinfo.h"

ImageInfo::ImageInfo(ImageData *newImageData,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageInfo)
{
    ui->setupUi(this);
    imageData = newImageData;
    formatHandler = new FormatHandler();

    setMouseTracking(true);
    isClicked = 0;
    isClickedOrig = 0;
    latitudeCandidate = 1000;
    longitudeCandidate = 1000;
    candidateIsCorrect = 0;
    candidatePointTime = new QDateTime;
    *candidatePointTime =  QDateTime::fromString(QString("0000:00:00 00:00:00"), "yyyy:MM:dd hh:mm:ss");

    markerLabel = new QLabel(ui->imageLabel);
    markerLabel->setPixmap(QPixmap::fromImage(QImage(":/js/images/marker-icon-red.png")));
    markerLabel->setVisible(false);

    markerSelectedLabel = new QLabel(ui->imageLabel);
    markerSelectedLabel->setPixmap(QPixmap::fromImage(QImage(":/js/images/marker-icon-gold.png")));
    markerSelectedLabel->setVisible(false);

    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    setStyleSheet(currentStyleSheet());
    ///////////////////////////////////
    setContextMenuPolicy(Qt::ActionsContextMenu);
    saveGpsAction = new QAction(QIcon(":/icons/saveGpsIconS.png"),tr("Save GPS coordinates to EXIF data of selected pictures"),this);
    saveGpsAction->setIconVisibleInMenu(true);
    this->addAction(saveGpsAction);

    saveDateTimeAction = new QAction(QIcon(":/icons/saveTimeIcon.png"),tr("Save date and time to EXIF data of selected pictures"),this);
    saveDateTimeAction->setIconVisibleInMenu(true);
    this->addAction(saveDateTimeAction);

    newCameraMarkerAction = new QAction(QIcon(":/js/images/marker-icon-gold.png"),tr("Set new camera location for selected pictures"),this);
    newCameraMarkerAction->setIconVisibleInMenu(true);
    this->addAction(newCameraMarkerAction);

    newObjectMarkerAction = new QAction(QIcon(":/js/images/marker-icon-green.png"),tr("Set new object location for selected pictures"),this);
    newObjectMarkerAction->setIconVisibleInMenu(true);
    this->addAction(newObjectMarkerAction);

    removeCameraMarkerAction = new QAction(tr("Remove camera marker location"), this);
    removeCameraMarkerAction->setIconVisibleInMenu(true);
    this->addAction(removeCameraMarkerAction);

    removeObjectMarkerAction = new QAction(tr("Remove object marker location"), this);
    removeObjectMarkerAction->setIconVisibleInMenu(true);
    this->addAction(removeObjectMarkerAction);

    setDirectionAction = new QAction(tr("Set camera direction"), this);
    setDirectionAction->setIconVisibleInMenu(true);
    this->addAction(setDirectionAction);

    openExternaly = new QAction(tr("Open Image in External Editor"));
    openExternaly->setIconVisibleInMenu(true);
    this->addAction(openExternaly);

    synchAction = new QAction(QIcon(":/icons/synch.png"),tr("Synchronize with GPS route"),this);
    synchAction->setIconVisibleInMenu(true);
    this->addAction(synchAction);

    ui->nameLabel->setWordWrap(true);
    ui->gpsLabel->setWordWrap(true);
    ui->timeLabel->setWordWrap(true);
    ui->altitudeLabel->setWordWrap(true);

    ui->nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->gpsLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->timeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    synchCheckBox = NULL;

    connect(this, SIGNAL(saveExifGps(QString, double, double, double, double, double, double)),
            imageData->exifRW, SLOT(saveExifGps(QString,double,double, double, double, double, double)));
    connect(this, SIGNAL(saveExifTime(QString,QDateTime*)), imageData->exifRW, SLOT(saveExifTime(QString,QDateTime*)));
    connect(openExternaly, SIGNAL(triggered()), this, SLOT(openExternalEditor()));
    connect(imageData, SIGNAL(imageReloadDone()), this, SLOT(imageChanged()));
}

void ImageInfo::retranslateUi() {
    setGpsLabel();
    setTimeLabel();

    saveGpsAction->setText(tr("Save GPS coordinates to EXIF data of selected pictures"));
    saveDateTimeAction->setText(tr("Save date and time to EXIF data of selected pictures"));
    newCameraMarkerAction->setText(tr("Set new camera location for selected pictures"));
    newObjectMarkerAction->setText(tr("Set new object location for selected pictures"));
    removeCameraMarkerAction->setText(tr("Remove camera marker location"));
    removeObjectMarkerAction->setText(tr("Remove object marker location"));
    setDirectionAction->setText( tr("Set camera direction"));
    synchAction->setText(tr("Synchronize with GPS route"));
}

ImageInfo::~ImageInfo() {
    if(imageData->isGps) {
        emit deleteMarker(imageData->id);
    }
    delete candidatePointTime;
    delete saveDateTimeAction;
    delete saveGpsAction;
    delete newCameraMarkerAction;
    delete newObjectMarkerAction;
    delete removeCameraMarkerAction;
    delete removeObjectMarkerAction;
    delete setDirectionAction;
    delete synchAction;
    delete ui;
}

QString ImageInfo::currentStyleSheet() {
    if(isClicked){
        markerLabel->setVisible(0);
        markerSelectedLabel->setVisible(imageData->isGps);
    } else {
        markerSelectedLabel->setVisible(0);
        markerLabel->setVisible(imageData->isGps);
    }

    QString borderColor;
    if(imageData->isGpsSaved == 0 || imageData->isDateTimeSaved == 0) { //neco neni ulozene
        borderColor = "#FF4444";
    } else if(isClicked) {
        borderColor = "#232323";    //tmavsi seda
    } else {
        borderColor =  "#838383";   //svetlejsi seda
    }
    QString sSheet;
    sSheet += ".QFrame { "
            + QString("background-color: %1;").arg(isClicked ? "#D0D0E7" : "#FFFFFF")
            + QString("border-width: 1px;")
            + "border-style: outset; "
            + "border-radius: 9px; "
            + QString("border-color: %1; }").arg(borderColor)

            + QString("#nameLabel { color:%1;}").arg("#232323")//.arg(pictureName)
            + QString("#timeLabel { color:%1}").arg(imageData->isDateTimeSaved ? "#232323" : "#FF0000")
            + QString("#gpsLabel { color:%1}").arg(imageData->isGpsSaved ? "#232323" : "#FF0000")
            + QString("#gpsObjLabel { color:%1}").arg(imageData->isGpsSaved ? "#232323" : "#FF0000")
            + QString("#objectPositionLabel { color:%1}").arg(imageData->isGpsSaved? "#232323" : "#FF0000")
            + QString("#directionLabel { color:%1}").arg(imageData->isGpsSaved ? "#232323" : "#FF0000")
            + QString("#altitudeLabel { color:%1}").arg((imageData->altitude < -999 || imageData->isGpsSaved) ? "#232323" : "#FF0000")

            + QString("#nameLabel:hover { background-color: #E1ECEC;}")
            + QString("#timeLabel:hover { background-color: #E1ECEC;}")
            + QString("#gpsLabel:hover { background-color: #E1ECEC;}")
            + QString("#altitudeLabel:hover { background-color: #E1ECEC;}")
            + QString("#frameImage:hover { background-color: #E1ECEC;}")
            + QString("#objectPositionLabell:hover { background-color: #E1ECEC;}")
            + QString("#directionLabell:hover { background-color: #E1ECEC;}")
            ;

    return sSheet;
}


QString ImageInfo::gpsString() {

    if(imageData->latitude == 1000) {
        return tr("-");
    } else {
        return formatHandler->gpsAllInFormat(imageData->latitude, imageData->longitude);
    }
}

QString ImageInfo::gpsCandidadesString() {
    return formatHandler->gpsAllInFormat(latitudeCandidate, longitudeCandidate);
}

void ImageInfo::setGpsLabel() {

    if (imageData->isGps) {
        if (isClicked){
            markerLabel->setVisible(0);
            markerSelectedLabel->setVisible(imageData->isGps);
        } else {
            markerSelectedLabel->setVisible(0);
            markerLabel->setVisible(imageData->isGps);
        }
    }
    switch (imageData->gpsSource) {
        case 1:
            ui->gpsLabel->setText(gpsString());
        break;
        case 2:
            //ui->gpsLabel->setTextFormat(Qt::RichText);
            ui->gpsLabel->setText("<img src=:/icons/chodecMini.png> " + gpsString());
        break;
        case 3:
            //ui->gpsLabel->setTextFormat(Qt::RichText);
            ui->gpsLabel->setText("<img src=:/icons/handIcon.png> " + gpsString());
        break;
        default:
        break;
    }

    if(imageData->latitude > 999) {
        ui->gpsLabel->setToolTip(tr("Unknown GPS coordinates"));
    } else {
        ui->gpsLabel->setToolTip(ui->gpsLabel->text());
    }
    ui->gpsLabel->repaint();
}

void ImageInfo::setAltitudeLabel() {
    if(imageData->altitude < -999) {
        ui->altitudeLabel->setText(tr("-"));
        ui->altitudeLabel->setToolTip(tr("Unknown altitude"));
        return;
    } else {
        switch (imageData->gpsSource) {
            case 1:
                ui->altitudeLabel->setText(QString(tr("%1m")).arg(imageData->altitude));
            break;
            case 2:
                ui->altitudeLabel->setText("<img src=:/icons/chodecMini.png> "
                                           + QString(tr("%1m")).arg(imageData->altitude));
            break;
            case 3:
                ui->altitudeLabel->setText("<img src=:/icons/handIcon.png> "
                                           + QString(tr("%1m")).arg(imageData->altitude));
            break;
            default:
            break;
        }
    }
    ui->altitudeLabel->setToolTip(ui->altitudeLabel->text());
}

void ImageInfo::setDirectionLabel() {
    QString directionText = QString(tr("%1°")).arg(imageData->direction);
    ui->directionLabel->setText(directionText);
    ui->directionLabel->setVisible(!qIsNaN(imageData->direction));

}

void ImageInfo::setObjectPositionLabel() {

    QString objPositionText = tr("-");
    ui->objectPositionLabel->setVisible(imageData->objLatitude != 1000);
    if (imageData->objLatitude != 1000) {
        objPositionText = QString(tr("Object %1"))
                .arg(formatHandler->gpsAllInFormat(imageData->objLatitude, imageData->objLongitude));
    }

    ui->objectPositionLabel->setText(objPositionText);
}

void ImageInfo::setIconSize(int iconS) {
    iconSize = iconS;
}

void ImageInfo::setTextLabels() {
    ui->nameLabel->setText(QFileInfo(imageData->pictureName).fileName());
    ui->nameLabel->setToolTip(QFileInfo(imageData->pictureName).fileName());
    setTimeLabel();
    setGpsLabel();
    setAltitudeLabel();
    setDirectionLabel();
    setObjectPositionLabel();
}

void ImageInfo::mouseDoubleClickEvent(QMouseEvent * event )
{
    event->accept();
    ImageView *i = new ImageView;
    i->setImage(imageData->pictureName);
    i->show();
}

void ImageInfo::setAltitude(double alt) {
    imageData->altitude = alt;
}

void ImageInfo::setObjectGpsFromMap(int id, double lat, double lon, double alt) {
    if(id == imageData->id) {
        imageData->objLatitude = lat;
        imageData->objLongitude = lon;
        imageData->isGpsSaved = 0;
        setStyleSheet(currentStyleSheet());
        setObjectPositionLabel();
    }

}

void ImageInfo::setCameraDirectionFromMap(int id, double direction, double angleOfView) {
    if (id == imageData->id) {
        imageData->direction = direction;
        imageData->angleOfView = angleOfView;
        imageData->isGpsSaved = 0;
        setStyleSheet(currentStyleSheet());
        setDirectionLabel();
    }
}



void ImageInfo::setGpsFromMap(int id, double lat, double lon, double alt) {

    //    qDebug()  << "setGpsFromMap" << iid << lat << lon << alt;

    if(id == imageData->id) {
        imageData->latitude = lat;
        imageData->longitude = lon;
        imageData->altitude = alt;
        imageData->gpsSource = 3;
        imageData->isGps = 1;
        imageData->isGpsSaved = 0;
        setStyleSheet(currentStyleSheet());
        setGpsLabel();
        setAltitudeLabel();
    }
}

void ImageInfo::setGpsCandidates(double lat, double lon, double alt, QDateTime pointTime, int method) {
    latitudeCandidate = lat;
    longitudeCandidate = lon;
    altitudeCandidate = alt;

    *candidatePointTime = pointTime;
    approxMethod = method;
}

void ImageInfo::setCandidateIsCorrect(bool isCorrect) {
    candidateIsCorrect = isCorrect;
}

void ImageInfo::setCandidateRouteName(QString routeN) {
    routeName = routeN;
}

void ImageInfo::changeDateTime(int timeShift, bool isSaveTime) {
    if (imageData->dateTime->isNull()
            || imageData->dateTime->toSecsSinceEpoch() == QDateTime::fromString(QString("0000:00:00 00:00:00"), "yyyy:MM:dd hh:mm:ss").toSecsSinceEpoch()) {
        return;
    }

    *imageData->dateTime = imageData->dateTime->addSecs(timeShift);
    imageData->isDateTimeChanged = 1;

    setTimeLabel();
    ui->timeLabel->repaint();

    if(isSaveTime) {
        saveDateTime();
    } else {
        imageData->isDateTimeSaved = 0;
        setStyleSheet(currentStyleSheet());
    }
}

void ImageInfo::setTimeLabel() {
    if (!imageData->dateTime->isNull()) {
        if(imageData->isDateTimeChanged) {
            ui->timeLabel->setText("<img src=:/icons/clock.png> " + imageData->dateTime->toString(formatHandler->formatDateTime));
        } else {
            ui->timeLabel->setText(imageData->dateTime->toString(formatHandler->formatDateTime));
        }
        ui->timeLabel->setToolTip(ui->timeLabel->text());
    } else {
        ui->timeLabel->setText(tr("-"));
        ui->timeLabel->setToolTip(tr("Unknown time"));
    }

}

void ImageInfo::resizeWidget(int size) {
    iconSize = size;

    ui->imageLabel->setFixedSize(imageSizeHint());
    ui->nameLabel->setFixedWidth(imageSizeHint().width());
    ui->gpsLabel->setFixedWidth(imageSizeHint().width());
    ui->timeLabel->setFixedWidth(imageSizeHint().width());
    ui->altitudeLabel->setFixedWidth(imageSizeHint().width());

    ui->imageLabel->resize(imageSizeHint());    //setPixmap(QPixmap::fromImage(*image_small->resize(100,100)));

    if(imageData->image_small->width() > imageData->image_small->height() ) {
        ui->imageLabel->setPixmap(QPixmap::fromImage(*imageData->image_small).scaledToWidth(imageSizeHint().width(), Qt::SmoothTransformation));
    } else {
        ui->imageLabel->setPixmap(QPixmap::fromImage(*imageData->image_small).scaledToHeight(imageSizeHint().height(), Qt::SmoothTransformation));
    }

    markerLabel->setVisible(imageData->isGps);
}

void ImageInfo::scaleFinished(bool ret) {
    if (ret) {
        ui->imageLabel->setPixmap(QPixmap::fromImage(*imageData->image_small));
        resizeWidget(iconSize);

        ui->imageLabel->repaint();
    } else {
        deleteLater();
    }
}
void ImageInfo::rescaleFinished(QPixmap *p) {
    ui->imageLabel->setPixmap(*p);
}

void ImageInfo::imageChanged() {
    ui->imageLabel->setPixmap(QPixmap::fromImage(*imageData->image_small));
    resizeWidget(iconSize);

    ui->imageLabel->repaint();
}

QSize ImageInfo::sizeHint() {
    return QSize(0,0);
}

QSize ImageInfo::imageSizeHint() {
    int x = imageData->scaleSize.width() * (0.3 + (iconSize*0.1));
    int y = imageData->scaleSize.height() * (0.3 + (iconSize*0.1));
    return QSize(x,y);
}

void ImageInfo::setLabels() {
    setTextLabels();
    resizeWidget(iconSize);
    if(imageData->isGps) {
        emit mapAddCameraMarker(imageData->id, imageData->latitude, imageData->longitude, imageData->direction, imageData->angleOfView);
    }
    qDebug() << imageData->objLatitude;
    if (imageData->objLatitude != 1000) {
        emit mapAddObjectMarker(imageData->id, imageData->objLatitude, imageData->objLongitude);
    }
    markerLabel->setVisible(imageData->isGps);
}

void ImageInfo::select() {
    isClickedOrig = 1;
    click(0);
    if(imageData->isGps) {
        emit selected(imageData->id,1);
    }
}

void ImageInfo::unselect() {
    isClickedOrig = 0;
    unclick();
    if(imageData->isGps) {
        emit selected(imageData->id,0);
    }
}

void ImageInfo::click(bool focus) {
    isClicked = 1;
    this->setStyleSheet(currentStyleSheet());

    if(focus) {
        emit getFocus(this->pos().y(), this->height());
    }
}

void ImageInfo::unclick() {
    isClicked = 0;
    this->setStyleSheet(currentStyleSheet());
}

void ImageInfo::checkMarkerClickedId(int iid) {
    if(iid == imageData->id) {
        click(1);
    } else if(!(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        unclick();
    }
}

int ImageInfo::actualWidth() {
    QMargins margins = ui->frameImage->layout()->contentsMargins();
    return imageSizeHint().width() + margins.left() + margins.right();
}


void ImageInfo::setGpsFormat(QAction *action) {
    formatHandler->formatGps = action->data().toString();
    setGpsLabel();
}

void ImageInfo::setDateTimeFormat(QAction *action) {
    formatHandler->formatDateTime = action->data().toString();
    setTimeLabel();
}

void ImageInfo::saveNewData(bool isSaveExif) {
    if(synchCheckBox->isChecked()) {
        //////
        imageData->latitude = latitudeCandidate;
        imageData->longitude = longitudeCandidate;
        imageData->altitude = altitudeCandidate;
        imageData->gpsSource = 2;
        imageData->isGps = 1;
        emit mapAddCameraMarker(imageData->id, imageData->latitude, imageData->longitude, imageData->direction, imageData->angleOfView);
        // FIXME mapAddObjectMarker()s
        //////////

        if (isSaveExif) {
            emit saveExifGps(imageData->pictureName, imageData->latitude, imageData->longitude, imageData->altitude, imageData->objLatitude, imageData->objLongitude, imageData->direction);
        } else {
            imageData->isGpsSaved = 0;
            setStyleSheet(currentStyleSheet());
        }
        setGpsLabel();
        setAltitudeLabel();

    }
}
void ImageInfo::saveGps() {
    if(imageData->isGps) {
        emit saveExifGps(imageData->pictureName, imageData->latitude, imageData->longitude, imageData->altitude, imageData->objLatitude, imageData->objLongitude, imageData->direction);
        imageData->isGpsSaved = 1;
        setStyleSheet(currentStyleSheet());
    }
}

void ImageInfo::saveDateTime() {
    if (!imageData->dateTime->isNull()
            && imageData->dateTime->toSecsSinceEpoch() != QDateTime::fromString(QString("0000:00:00 00:00:00"), "yyyy:MM:dd hh:mm:ss").toSecsSinceEpoch()) {
        emit saveExifTime(imageData->pictureName, imageData->dateTime);
        imageData->isDateTimeSaved = 1;
        imageData->lastDateTimeSaved = *imageData->dateTime;
        setStyleSheet(currentStyleSheet());
    }
}

void ImageInfo::setOrigDateTime (bool isSaveTime) {
    *imageData->dateTime = imageData->originalDateTime;
    imageData->isDateTimeChanged = 0;
    if(imageData->dateTime->toSecsSinceEpoch() != imageData->lastDateTimeSaved.toSecsSinceEpoch()) {
        if(isSaveTime) {
            saveDateTime();
        } else {
            imageData->isDateTimeSaved = 0;
        }
    } else {
        imageData->isDateTimeSaved = 1;
    }
    setTimeLabel();
    setStyleSheet(currentStyleSheet());
}

QStringList ImageInfo::exifInformation() {
    QStringList exifList;
    exifList << QFileInfo(imageData->pictureName).fileName()
             << QFileInfo(imageData->pictureName).absolutePath() + "/";
    exifList.append(*imageData->exifRW->readExifInfo(imageData->pictureName, formatHandler));
    return exifList;
}

void ImageInfo::changeLabelVisibility(QAction *a) {
    if(a->data().toString() == "dateTime") {
        ui->timeLabel->setVisible(a->isChecked());
    } else if(a->data().toString() == "latLon") {
        ui->gpsLabel->setVisible(a->isChecked());
    } else if(a->data().toString() == "alt") {
        ui->altitudeLabel->setVisible(a->isChecked());
    }
}

void ImageInfo::openExternalEditor() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(imageData->pictureName).absoluteFilePath()));
}
