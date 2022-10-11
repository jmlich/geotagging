/** @file gpssegment.cpp
 * Soubor s tridou GpsSegment dedici ze tridy QList<GpsPoint *>
 * reprezentujici segment trasy obsahujici seznam bodu
 */

#include "gpssegment.h"

GpsSegment::GpsSegment()
{
}

qint64 GpsSegment::startTime()
{
    return first()->dateTime->toSecsSinceEpoch();
}

qint64 GpsSegment::endTime()
{
    return last()->dateTime->toSecsSinceEpoch();
}

double GpsSegment::startLat()
{
    return first()->latitude;
}

double GpsSegment::startLon()
{
    return first()->longitude;
}
