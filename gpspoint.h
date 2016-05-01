/** @file gpspoint.h
  * Hlavickovy soubor tridy GpsPoint reprezentujici bod trasy
  */

#ifndef GPSPOINT_H
#define GPSPOINT_H
#include<QString>
#include<QRegExp>
#include <QDateTime>

class GpsPoint
{
public:
    GpsPoint();

    double latitude;
    double longitude;
    QDateTime *dateTime;
    double ele;

    void setTime(QString time);
    void changeDateTime(int year, int month, int day, int seconds);
};

#endif // GPSPOINT_H
