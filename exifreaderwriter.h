/** @file exifinfo.h
 * Hlavickovy soubor tridy ExifReaderWriter dedici ze trify QObject pro nacitani a zapis Exif metadat fotografie
 */

#ifndef EXIFREADERWRITER_H
#define EXIFREADERWRITER_H

#include "formathandler.h"
#include <QDateTime>
#include <QMessageBox>
#include <QObject>
#include <exiv2/exiv2.hpp>
#include <math.h>
class ExifReaderWriter : public QObject {
    Q_OBJECT

    QDateTime* readExifDate(Exiv2::ExifData& exifData, std::string keyStr);
    double readAltitude(QString str, Exiv2::ExifData& exifData);
    double readLatLon(QString str, Exiv2::ExifData& exifData);
    void writeData(Exiv2::ExifData& exifData, std::string keyStr, QString str);
    void removeData(Exiv2::ExifData& exifData, std::string key);

    QString exifLatLonString(double l);
    std::unique_ptr<Exiv2::Image> openExif(QString pictureName);
    QString readExifItem(Exiv2::ExifData& exifData, std::string keyStr);
    double readExifItemDouble(Exiv2::ExifData& exifData, std::string keyStr);
    QString getExposureTime(Exiv2::ExifData& exifData);
    QString getExposureBias(Exiv2::ExifData& exifData);
    QString getExposureProgram(Exiv2::ExifData& exifData);
    QString getMeteringMode(Exiv2::ExifData& exifData);
    QString getFNumber(Exiv2::ExifData& exifData);
    QString getFocalLength(Exiv2::ExifData& exifData);
    QString getFlash(Exiv2::ExifData& exifData);

public:
    explicit ExifReaderWriter(QObject* parent = 0);
    QStringList* readExifInfo(QString pictureName, FormatHandler* formatH);
    QString timeFormat;

signals:
    void setGps(double lat, double lon, double alt, double direction, double angleOfView);
    void setObjGps(double lat, double lon);
    void finished();
    void setDateTime(QDateTime dateTime);
public slots:
    void readExif(QString pictureName);
    void saveExifGps(QString pictureName, double latitude, double longitude, double alitude, double objLatitude, double objLongitude, double direction);
    void saveExifTime(QString pictureName, QDateTime* dateTime);
};

#endif // EXIFREADERWRITER_H
