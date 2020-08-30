/** @file gpssegment.cpp
  * Soubor s tridou GpsSegment dedici ze tridy QList<GpsPoint *>
  * reprezentujici segment trasy obsahujici seznam bodu
  */

#include "gpssegment.h"

GpsSegment::GpsSegment()
{
}

uint GpsSegment::startTime() {
    return first()->dateTime->toTime_t();
}

uint GpsSegment::endTime() {
    return last()->dateTime->toTime_t();
}

double GpsSegment::startLat() {
    return first()->latitude;
}

double GpsSegment::startLon() {
    return first()->longitude;
}
