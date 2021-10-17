/** @file gpspoint.cpp
  * Soubor s tridou GpsPoint reprezentujici bod trasy
  */

#include "gpspoint.h"

GpsPoint::GpsPoint() {
    dateTime = new QDateTime;
    *dateTime = QDateTime::fromString(QString("0000:00:00 00:00:00"), "yyyy:MM:dd hh:mm:ss");
    ele = -1000;
}

void GpsPoint::setTime(QString time_str) {
    QDateTime dt = QDateTime::fromString(time_str, Qt::ISODate);
    if(dt.isValid()) {
        *dateTime = dt;
    }
}

void GpsPoint::changeDateTime(int year, int month, int day, int seconds) {
    *dateTime = dateTime->addYears(year);
    *dateTime = dateTime->addMonths(month);
    *dateTime = dateTime->addDays(day);
    *dateTime = dateTime->addSecs(seconds);
}
