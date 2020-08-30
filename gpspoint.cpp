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
    QRegExp rx("(\\d\\d\\d\\d-\\d\\d-\\d\\d)T(\\d\\d:\\d\\d:\\d\\d)Z");
    rx.indexIn(time_str, 0);

    dateTime->setDate(QDate::fromString(rx.cap(1), "yyyy-MM-dd"));
    dateTime->setTime(QTime::fromString(rx.cap(2), "hh:mm:ss"));

}

void GpsPoint::changeDateTime(int year, int month, int day, int seconds) {
    *dateTime = dateTime->addYears(year);
    *dateTime = dateTime->addMonths(month);
    *dateTime = dateTime->addDays(day);
    *dateTime = dateTime->addSecs(seconds);
}
