/** @file imagedata.h
  * Hlavickovy soubor tridy ImageData dedici ze tridy QObject obsahujici data nactena z fotografie
  */

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QObject>
#include <QDateTime>
#include <QImage>
#include <QImageReader>
#include "exifreaderwriter.h"
class ImageData : public QObject
{
    Q_OBJECT
public:
    explicit ImageData(QObject *parent = 0);
    int id;

    int gpsSource;  //1 = z fotografie
                    //2 = prirazene z trasy
                    //3 = prirazene rucne

    QString pictureName;
    QDateTime *dateTime;
    QDateTime originalDateTime;
    QDateTime lastDateTimeSaved;
    bool isDateTimeChanged;
    bool isGps;
    bool isDateTimeSaved;
    bool isGpsSaved;
    double latitude;
    double longitude;
    double altitude;

    QImage *image_small;
    int loadData(QString pictureFName);
    void scaleImage(QString pictureName);
    QSize scaleSize;
    ExifReaderWriter *exifRW;
signals:
     void readExif(QString n);
public slots:
     void setDateTime(QDateTime dateTime);
     void setGps(double lat, double lon, double alt);


};

#endif // IMAGEDATA_H
