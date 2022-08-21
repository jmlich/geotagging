/** @file exifinfo.cpp
  * Soubor s tridou ExifReaderWriter dedici ze trify QObject pro nacitani a zapis Exif metadat fotografie
  */

#include "exifreaderwriter.h"
#include <QDebug>
#include <QRegularExpression>

ExifReaderWriter::ExifReaderWriter(QObject *parent) :
        QObject(parent)
{
}

std::unique_ptr<Exiv2::Image> ExifReaderWriter::openExif(QString pictureName) {
    std::unique_ptr<Exiv2::Image> image;
    try{
#ifdef _WIN32
        image = Exiv2::ImageFactory::open( std::string(pictureName.toLocal8Bit()));
#else
        image = Exiv2::ImageFactory::open( std::string(pictureName.toUtf8()));
#endif
    } catch (Exiv2::Error& e) {
        Q_UNUSED(e)
//        qDebug() << e.what();
        return image;
    }

    if(image.get()){
        image->readMetadata();
    }
    return image;

}

double ExifReaderWriter::readAltitude(QString str, Exiv2::ExifData &exifData) {
    QString altStr = readExifItem(exifData,str.toStdString());
    QString altStrRef = readExifItem(exifData,str.toStdString() + "Ref");
    if(!altStr.isEmpty()) {
        QRegularExpression rx("^(\\d+)/(\\d+)$");
        QRegularExpressionMatch match = rx.match(altStr);
        double alt = match.captured(1).toDouble()/match.captured(2).toDouble();
        if(altStrRef == "1") {
            alt *= -1;
        }
        return alt;
    } else {
        return -1000;
    }
}

double ExifReaderWriter::readLatLon(QString str, Exiv2::ExifData &exifData) {

    QString gpsStr = readExifItem(exifData,str.toStdString());
    QString gpsStrRef = readExifItem(exifData,str.toStdString() + "Ref");

    if(!gpsStr.isEmpty()) {
        //format stupne/jmenovatel minuty/jmenovatel sekundy/jmenovatel
        QRegularExpression rx("^(\\d+)/(\\d+) (\\d+)/(\\d+) (\\d+)/(\\d+)$");
        QRegularExpressionMatch match = rx.match(gpsStr);
        double gps =
                (match.captured(1).toDouble()/match.captured(2).toDouble())
                + (
                    (match.captured(3).toDouble()/match.captured(4).toDouble()) +
                    (match.captured(5).toDouble()/match.captured(6).toDouble())/60
                    ) / 60;
        if (gpsStrRef == "S" || gpsStrRef == "W") {
            gps *= -1;
        }
        return gps;
    } else {
        return 1000;
    }
}


void ExifReaderWriter::readExif(QString pictureName) {
    std::unique_ptr<Exiv2::Image> image = openExif(pictureName);
    if(image.get() == 0) {
        return;
    }
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        //qDebug() << "nejsou exif data";
        emit finished();
        return;
    }

    //cteniGPS souradnic
    double lat = readLatLon("Exif.GPSInfo.GPSLatitude", exifData);
    double lon = readLatLon("Exif.GPSInfo.GPSLongitude", exifData);
    double alt = readAltitude("Exif.GPSInfo.GPSAltitude", exifData);
    double direction = readExifItemDouble(exifData, "Exif.GPSInfo.GPSImgDirection");

//     (Resolution in pixels/Focal plane resolution in dpi) X 25.4(mm/in)=size in mm
    double focalPlaneXRes = readExifItemDouble(exifData, "Exif.Photo.FocalPlaneXResolution");
    double xRes = readExifItemDouble(exifData, "Exif.Image.XResolution");
    const double inchToMilimeter = 25.4;
    double sensorWidth = (xRes / focalPlaneXRes) * inchToMilimeter;
    double focalLength = readExifItemDouble(exifData, "Exif.Photo.FocalLength");
//     Angle of view (in degrees) = 2 ArcTan( sensor width / (2 X focal length)) * (180/π)
    double angleOfView = std::atan2( sensorWidth, 2*focalLength) * (180 / M_PI);

    double objLat = readLatLon("Exif.GPSInfo.GPSDestLatitude", exifData);
    double objLon = readLatLon("Exif.GPSInfo.GPSDestLongitude", exifData);

//    qDebug() << "focalPlaneXRes" << focalPlaneXRes
//             << "xRes " << xRes
//             << "sensorWidth " << sensorWidth
//             << "focalLength " << focalLength
//             << "angleOfView " << angleOfView;

    qDebug() << "loaded exif: " << pictureName << lat << lon << alt
             << "direction, fov: "<< direction << angleOfView
             << "object: " << objLat << objLon;

    if(lat < 1000 && lon<1000) {
        emit setGps(lat, lon, alt, direction, angleOfView);
    }
    if (objLat < 1000 && objLon < 1000) {
        emit setObjGps(objLat, objLon);
    }

    ////////////////////////////

    QDateTime *dateTime = NULL;
    //cteni data
    if((dateTime = readExifDate(exifData,"Exif.Photo.DateTimeOriginal")) == NULL) {
        if((dateTime = readExifDate(exifData,"Exif.Image.DateTimeOriginal")) == NULL) {
            if((dateTime = readExifDate(exifData,"Exif.Photo.DateTimeDigitized")) == NULL) {
                dateTime = readExifDate(exifData,"Exif.Image.DateTime");
            }
        }
    }

    if(dateTime != NULL) {
        emit setDateTime(*dateTime);
    }
    emit finished();
    return;
}

QDateTime *ExifReaderWriter::readExifDate(Exiv2::ExifData &exifData, std::string keyStr) {

    QDateTime *dateTime = NULL;
    QString strDate = readExifItem(exifData,keyStr);
    if(!strDate.isEmpty()) {
        if(!QDateTime::fromString(strDate, "yyyy:MM:dd hh:mm:ss").isNull()) {
            timeFormat = "yyyy:MM:dd hh:mm:ss";
            dateTime = new QDateTime(QDateTime::fromString(strDate,timeFormat));
        } else if(!QDateTime::fromString(strDate, "yyyy:MM:dd:hh:mm:ss").isNull()) {
            timeFormat = "yyyy:MM:dd:hh:mm:ss";
            dateTime = new QDateTime(QDateTime::fromString(strDate,timeFormat));
        }
    }
    return dateTime;

}

void ExifReaderWriter::saveExifTime(QString pictureName, QDateTime *dateTime)
{
    std::unique_ptr<Exiv2::Image> image = openExif(pictureName);
    if(image.get() == 0) {
        return;
    }

    if(image->exifData().empty()) {
        Exiv2::ExifData exifDataTmp;
        image->setExifData(exifDataTmp);
    }

    Exiv2::ExifData &exifData = image->exifData();

    QString str = dateTime->toString(timeFormat.toLatin1());

    writeData(exifData, "Exif.Image.DateTime", str);
    writeData(exifData, "Exif.Image.DateTimeOriginal", str);
    writeData(exifData, "Exif.Photo.DateTimeOriginal", str);
    writeData(exifData, "Exif.Photo.DateTimeDigitized", str);

    image->writeMetadata();
}

QString ExifReaderWriter::exifLatLonString(double l) {
    if(l < 0) {
        l = -l;
    }
    int degrees = floor(l);
    double minTmp = (l - degrees) * 60;
    int min = floor(minTmp);
    int secM = floor((minTmp - min) * 60 * 1000);
    return QString("%1/1 %2/1 %3/1000").arg(degrees).arg(min).arg(secM);
}

void ExifReaderWriter::saveExifGps(QString pictureName, double latitude, double longitude, double altitude, double objLatitude, double objLongitude, double direction) {
    qDebug() << "saveExifGps" << pictureName << latitude << longitude << altitude << objLatitude << objLongitude << direction;
    std::unique_ptr<Exiv2::Image> image = openExif(pictureName);
    if (image.get() == 0) {
        return;
    }

    if(image->exifData().empty()) {
        Exiv2::ExifData exifDataTmp;
        image->setExifData(exifDataTmp);
    }

    Exiv2::ExifData &exifData = image->exifData();

    if (latitude != 1000) {
        writeData(exifData, "Exif.GPSInfo.GPSLatitude", exifLatLonString(latitude));
        writeData(exifData, "Exif.GPSInfo.GPSLongitude", exifLatLonString(longitude));
        writeData(exifData, "Exif.GPSInfo.GPSLatitudeRef", (latitude<0 ? "S" : "N"));
        writeData(exifData, "Exif.GPSInfo.GPSLongitudeRef", (longitude<0 ? "W" : "E"));
    } else {
        removeData(exifData, "Exif.GPSInfo.GPSLatitude");
        removeData(exifData, "Exif.GPSInfo.GPSLongitude");
        removeData(exifData, "Exif.GPSInfo.GPSLatitudeRef");
        removeData(exifData, "Exif.GPSInfo.GPSLongitudeRef");
    }

    if (qIsNaN(direction)) {
        removeData(exifData, "Exif.GPSInfo.GPSImgDirection");
    } else {
        qDebug()  << "FIXME write data " << direction;
        writeData(exifData, "Exif.GPSInfo.GPSImgDirection", QString("%1/1000").arg(int(direction*1000)));
    }

    if(altitude > -999) {
        writeData(exifData, "Exif.GPSInfo.GPSAltitude", (QString("%1/%2").arg(abs(round(altitude * 1000))).arg(1000)));
        writeData(exifData, "Exif.GPSInfo.GPSAltitudeRef", (altitude<0 ? "1" : "0"));
    } else {
        removeData(exifData, "Exif.GPSInfo.GPSAltitude");
        removeData(exifData, "Exif.GPSInfo.GPSAltitudeRef");
    }

    if (objLatitude != 1000) {
        qDebug() << "Storing object position";
        writeData(exifData, "Exif.GPSInfo.GPSDestLatitude", exifLatLonString(objLatitude));
        writeData(exifData, "Exif.GPSInfo.GPSDestLongitude", exifLatLonString(objLongitude));
        writeData(exifData, "Exif.GPSInfo.GPSDestLatitudeRef", (objLatitude<0 ? "S" : "N"));
        writeData(exifData, "Exif.GPSInfo.GPSDestLongitudeRef", (objLongitude<0 ? "W" : "E"));
    } else {
        removeData(exifData, "Exif.GPSInfo.GPSDestLatitude");
        removeData(exifData, "Exif.GPSInfo.GPSDestLongitude");
        removeData(exifData, "Exif.GPSInfo.GPSDestLatitudeRef");
        removeData(exifData, "Exif.GPSInfo.GPSDestLongitudeRef");
    }


    image->writeMetadata();
}



void ExifReaderWriter::writeData(Exiv2::ExifData &exifData, std::string keyStr, QString str) {
    try{
        Exiv2::ExifKey key(keyStr);
        Exiv2::ExifData::iterator pos = exifData.findKey(key);
        if(pos != exifData.end()){
            exifData[keyStr].setValue(str.toStdString());
        } else {    //vytvorim novou polozku
            exifData[keyStr].setValue(str.toStdString());
        }
    } catch (Exiv2::Error& e) {
        Q_UNUSED(e);
//        qDebug() << e.what();
    }

}

void ExifReaderWriter::removeData(Exiv2::ExifData &exifData, std::string keyStr) {
    try {
        Exiv2::ExifKey key(keyStr);
        Exiv2::ExifData::iterator pos = exifData.findKey(key);
        if(pos != exifData.end()){
            exifData.erase(pos);
        }

    } catch (Exiv2::Error& e) {
        qDebug() << e.what();
    }
}

double ExifReaderWriter::readExifItemDouble( Exiv2::ExifData &exifData, std::string keyStr ) {
    try{
        Exiv2::ExifKey key(keyStr);
        Exiv2::ExifData::iterator pos = exifData.findKey(key);

        if (pos != exifData.end()) {
            return exifData[keyStr].toFloat();
        }
    }
    catch (Exiv2::Error& e) {
        Q_UNUSED(e);
//        qDebug() << e.what();
    }

    return qQNaN();

}


QString ExifReaderWriter::readExifItem( Exiv2::ExifData &exifData, std::string keyStr) {
    try{
        Exiv2::ExifKey key(keyStr);
        Exiv2::ExifData::iterator pos = exifData.findKey(key);

        if (pos != exifData.end()) {
            return exifData[keyStr].toString().data();
        }
    }
    catch (Exiv2::Error& e) {
        Q_UNUSED(e);
//        qDebug() << e.what();
    }
    return "";

}

QString ExifReaderWriter::getExposureTime(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.ExposureTime");
    QStringList exifStrList = exifStr.split("/");

    if(exifStrList.length() == 2) {
        QString part1 = exifStrList.at(0);
        QString part2 = exifStrList.at(1);
        while(part1.endsWith("0") && part2.endsWith("0")) {
            part1.chop(1);
            part2.chop(1);
        }
        exifStr = part1 + "/" + part2 + " s";

    }
    return exifStr;
}

QString ExifReaderWriter::getExposureBias(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.ExposureBiasValue");

    QStringList exifStrList = exifStr.split("/");
    if(exifStrList.length() == 2) {
        double f = exifStrList.at(0).toDouble() / exifStrList.at(1).toDouble();
        return QString::number(f) + " EV";
    }
    return exifStr;

}

QString ExifReaderWriter::getMeteringMode(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.MeteringMode");
    switch (exifStr.toInt()){
    case 0:
        return tr("unknown");
    case 1:
        return tr("average");
    case 2:
        return tr("center weighted average");
    case 3:
        return tr("spot");
    case 4:
        return tr("multi-spot");
    case 5:
        return tr("multi-segment");
    case 6:
        return tr("partial");
    case 7:
        return tr("other");
    default:
        return "";

    }
}

QString ExifReaderWriter::getExposureProgram(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.ExposureProgram");

    switch (exifStr.toInt()){
    case 1:
        return tr("manual control");
    case 2:
        return tr("normal");
    case 3:
        return tr("aperture priority");
    case 4:
        return tr("shutter priority");
    case 5:
        return tr("program creative (slow program)");
    case 6:
        return tr("program action(high-speed program)");
    case 7:
        return tr("portrait mode");
    case 8:
        return tr("landscape mode");
    default:
        return "";

    }
}

QString ExifReaderWriter::getFNumber(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.FNumber");

    QStringList exifStrList = exifStr.split("/");
    if(exifStrList.length() == 2) {
        double f = exifStrList.at(0).toDouble() / exifStrList.at(1).toDouble();
        return "F" + QString::number(f);
    }
    return exifStr;
}

QString ExifReaderWriter::getFocalLength(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.FocalLength");

    QStringList exifStrList = exifStr.split("/");
    if(exifStrList.length() == 2){
        double f = exifStrList.at(0).toDouble() / exifStrList.at(1).toDouble();
        return  QString::number(f) + " mm";
    }
    return exifStr;

}

QString ExifReaderWriter::getFlash(Exiv2::ExifData &exifData) {
    QString exifStr = readExifItem(exifData, "Exif.Photo.Flash");//'0' means flash did not fire, '1' flash fired,
    if(exifStr.isEmpty()) {
        return "";
    }
    switch (exifStr.toInt() % 2){
    case 0:
        return tr("did not fire");
    case 1:
        return tr("fired");
    default:
        return "";
    }
}


QStringList *ExifReaderWriter::readExifInfo(QString pictureName, FormatHandler *formatH) {
    QStringList *exifList = new QStringList;

    std::unique_ptr<Exiv2::Image> image = openExif(pictureName);
    if(image.get() == 0) {
        return exifList;
    }
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        //nejsou exif data
        return exifList;
    }


    //cteniGPS souradnic
    double lat = readLatLon("Exif.GPSInfo.GPSLatitude", exifData);
    double lon = readLatLon("Exif.GPSInfo.GPSLongitude", exifData);
    double alt = readAltitude("Exif.GPSInfo.GPSAltitude", exifData);
    ////////////////////////////

    double destLat = readLatLon("Exif.GPSInfo.GPSDestLatitude", exifData);
    double destLon = readLatLon("Exif.GPSInfo.GPSDestLongitude", exifData);


    //cteni data
    QDateTime *dateTime = NULL;
    if((dateTime = readExifDate(exifData,"Exif.Photo.DateTimeOriginal")) == NULL) {
        if((dateTime = readExifDate(exifData,"Exif.Image.DateTimeOriginal")) == NULL) {
            if((dateTime = readExifDate(exifData,"Exif.Photo.DateTimeDigitized")) == NULL) {
                dateTime = readExifDate(exifData,"Exif.Image.DateTime");
            }
        }
    }


    double direction = readExifItemDouble(exifData, "Exif.GPSInfo.GPSImgDirection");
    double pitch = readExifItemDouble(exifData, "Exif.GPSInfo.GPSPitch");
    double roll = readExifItemDouble(exifData, "Exif.GPSInfo.GPSRoll");

// https://sourceforge.net/p/exiftool/code/ci/master/tree/lib/Image/ExifTool/Exif.pm#l4412
// https://www.thephotoforum.com/threads/calculate-angle-of-view-from-exif-tags.129742/
//    FOV = 2*arctan((SQRT(a*a + b*b)/2)/f)

    ////////////////////
    QString cameraMake = readExifItem(exifData, "Exif.Image.Make");
    QString cameraModel = readExifItem(exifData, "Exif.Image.Model");
    QString imageSize = readExifItem(exifData, "Exif.Photo.PixelXDimension")
                        +" x "
                        + readExifItem(exifData, "Exif.Photo.PixelYDimension");
    QString exposureTime = getExposureTime(exifData);
    QString flash = getFlash(exifData);
    //'5' flash fired but strobe return light not detected, '7' flash fired and strobe return light detected.
    QString meteringMode = getMeteringMode(exifData);
    QString fNumber = getFNumber(exifData);
    QString isoSpeed = readExifItem(exifData, "Exif.Photo.ISOSpeedRatings");
    QString focalLength = getFocalLength(exifData);
    QString comment = readExifItem(exifData, "Exif.Photo.UserComment");
    QString exposureBias = getExposureBias(exifData);
    QString exposureProgram = getExposureProgram(exifData);


    //////////////////
    (*exifList) << ((dateTime != NULL) ? dateTime->toString(formatH->formatDateTime) : "")
            << (lat < 1000 ?(formatH->gpsInFormat(lat) + (lat>=0 ? tr("N") : tr("S"))) : "")
            << (lon < 1000 ?(formatH->gpsInFormat(lon) + (lon>=0 ? tr("E") : tr("W"))) : "")
            << (alt > -999 ?(QString::number(alt) + tr(" m")) : "")
            << (qIsNaN(direction) ? "" : formatH->gpsInFormat(direction))
            << (qIsNaN(pitch) ? "" : formatH->gpsInFormat(pitch))
            << (qIsNaN(roll) ? "" : formatH->gpsInFormat(roll))
            << (destLat < 1000 ?(formatH->gpsInFormat(destLat) + (destLat>=0 ? tr("N") : tr("S"))) : "")
            << (destLon < 1000 ?(formatH->gpsInFormat(destLon) + (destLon>=0 ? tr("E") : tr("W"))) : "")
            << cameraMake
            << cameraModel
            << exposureTime
            << exposureBias
            << exposureProgram
            << imageSize
            << flash
            << fNumber
            << meteringMode
            << isoSpeed
            << focalLength
            << comment
            ;

    return exifList;

}
