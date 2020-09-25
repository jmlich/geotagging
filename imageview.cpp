/** @file imageview.cpp
  * SOubor s tridou tridy ImageView dedici ze tridy QWidget
  * pro zobrazeni noveho okna s fotografii v puvodni velikosti
  */

#include <QDebug>
#include "imageview.h"

ImageView::ImageView(QWidget *parent):
    QWidget(parent)
{
    imageL = new QLabel;
    scrollArea = new ScrollArea;
    this->setLayout(new QHBoxLayout);
    connect(scrollArea, SIGNAL(changeSize(int)), this, SLOT(changeImageSize(int)));

    this->layout()->setContentsMargins(0,0,0,0);
    this->layout()->addWidget(scrollArea);
    this->setWindowModality(Qt::ApplicationModal);
    this->adjustSize();
}

ImageView::~ImageView() {
    delete img;
}

QSize ImageView::sizeHint() const
{
    return QSize(1024, 768);
}

void ImageView::setImage(QString pictureName) {

    img = new QImage(pictureName);
    if(img->isNull()){
        //img je null
        return;
    }
    Exiv2::Image::AutoPtr image;

    bool isExif = false;
    try{
        image = Exiv2::ImageFactory::open ( std::string(pictureName.toUtf8()));
        isExif = true;
    } catch (Exiv2::Error& e) {
        qDebug() << "Caught Exiv2 exception 2 '" << e.what() << "'";
        isExif = false;
    }

    if (isExif) {
        image.get();
        image->readMetadata();
        QTransform rot;
        Exiv2::ExifData &exifData =  image->exifData();
        if (!exifData.empty()) {
            Exiv2::ExifKey key("Exif.Image.Orientation");
            Exiv2::ExifData::iterator pos = exifData.findKey(key);
            if(pos != exifData.end()) {
                //qDebug() << exifData["Exif.Image.Orientation"].toString().data() << "iiii";
                QString str = exifData["Exif.Image.Orientation"].toString().data();
                switch(str.toInt()){
                    case 3: //obraz otoceny o 180stupnu
                        *img = img->transformed(rot.rotate(180));
                    break;
                    case 6: //obraz otoceny o 90stupnu
                        *img = img->transformed(rot.rotate(90));
                    break;
                    case 8: //obraz otoceny o 280stupnu
                        *img = img->transformed(rot.rotate(280));
                    break;
                    default:
                    break;
                }

            }
        }
    }
    this->setContentsMargins(QMargins(0,0,0,0));
    scrollArea->setContentsMargins(QMargins(0,0,0,0));

    currentSize = QSize(this->size().width()-2, this->size().height()-2);

    imageL->setPixmap(QPixmap::fromImage((img->scaled(currentSize,Qt::KeepAspectRatio))));

    scrollArea->setWidget(imageL);
    scrollArea->resize(this->size());
    resize(scrollArea->widget()->width() + 2, scrollArea->widget()->height() + 2);
    imageL->setAlignment(Qt::AlignCenter);
}

void ImageView::resizeEvent( QResizeEvent *event ) {
    event->accept();
    currentSize = QSize(this->size().width()-2, this->size().height()-2);
    imageL->setPixmap(QPixmap::fromImage((img->scaled(currentSize,Qt::KeepAspectRatio))));

    scrollArea->resize(this->size());

    //scrollArea->checkScrollBars();
}

/*void ImageView::wheelEvent(QWheelEvent *event)
{
    event->accept();
     QSize s;
    if(event->delta() < 0)
    {
        qDebug() <<imageL->width() << imageL->height() ;
        if(imageL->width() > 30 && imageL->height() > 30 )
            s = QSize(imageL->width()* 0.90, imageL->height() * 0.90);
        else
            return;

    }
    else
        s = QSize(imageL->width()* 1.1, imageL->height() * 1.1);
    qDebug() << s;
    imageL->setPixmap(QPixmap::fromImage((img->scaled(s,Qt::KeepAspectRatio))));
    imageL->resize(s);

}*/

void ImageView::changeImageSize(int delta) {
    if(delta < 0) {
        if(currentSize.width() > 60 && currentSize.height() > 60 ) {
            currentSize = QSize(currentSize.width() * 0.90, currentSize.height() * 0.90);
        } else {
            return;
        }
    } else {
        currentSize = QSize(currentSize.width() * 1.1, currentSize.height() * 1.1);
    }

    imageL->setPixmap(QPixmap::fromImage((img->scaled(currentSize,Qt::KeepAspectRatio))));
    scrollArea->setWidget(imageL);
}
