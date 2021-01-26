/** @file imagedata.h
  * Hlavickovy soubor tridy ImageData dedici ze tridy QObject obsahujici data nactena z fotografie
  */

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QObject>
#include <QDateTime>
#include <QImage>
#include <QImageReader>
#include <QFileSystemWatcher>
#include <QTimer>
#include "exifreaderwriter.h"

#define RELOAD_TIMER_INTERVAL 1500

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

    QImage *image_small = NULL;
    int loadData(QString pictureFName);
    void scaleImage(QString pictureName);
    QSize scaleSize;
    ExifReaderWriter *exifRW;

    QFileSystemWatcher *watcher;
    QTimer *reloadTimer;

signals:
     void readExif(QString n);
     void imageReloadDone();

public slots:
     void setDateTime(QDateTime dateTime);
     void setGps(double lat, double lon, double alt);

     void imageReload();
     void watchedFileChanged(const QString filename);


};

#endif // IMAGEDATA_H
