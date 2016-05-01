/** @file formathandler.cpp
  * Soubor s tridou FormatHandler dedici ze trify QObject pro prevadeni gps souradnic a casu
  * do pozadovaneho formatu
  */

#include "formathandler.h"

FormatHandler::FormatHandler() :
        QObject()
{
    formatGps = "dm";
    formatDateTime = "dd.M.yyyy hh:mm:ss";
}


QString FormatHandler::gpsInFormat(double l)
{
    QString lStr;
    if(formatGps=="d") //stupne
    {
        if(l<0)
            l *= -1;
        lStr = QString::number(l, 'f', 5);

    }
    else if(formatGps=="dm")   //stupne minuty
    {
        lStr = convertDegreeToDM(l);
    }
    else    //stupne minuty sekundy
    {
        lStr = convertDegreeToDMS(l);
    }
    return lStr;
}
QString FormatHandler::gpsAllInFormat(double lat, double lon)
{
    QString latStr = gpsInFormat(lat);
    QString lonStr = gpsInFormat(lon);
    return QString("%1%2 %3%4")
            .arg(latStr)
            .arg(lat>=0 ? tr("N ") : tr("S "))
            .arg(lonStr)
            .arg(lon>=0 ? tr("E") : tr("W"));
}

QString FormatHandler::convertDegreeToDM(double l)
{
    if(l < 0)
        l *= -1;
    int degree = l;
    double min = (l - degree) * 60;

    return QString::number(degree) + "°" + QString::number(min, 'f', 3) + "'";
}
QString FormatHandler::convertDegreeToDMS(double l)
{
    if(l < 0)
        l = l * (-1);
    int degree = l;
    double minTmp = (l - degree) * 60;

    int min = static_cast<int>(minTmp);
    double sec = (minTmp - min) * 60;
    return QString("%1°%2'%3\"").arg(degree).arg(min).arg(QString::number(sec, 'f', 2));
}
