/** @file gpssegment.h
  * Hlavickovy soubor tridy GpsSegment dedici ze tridy QList<GpsPoint *>
  * reprezentujici segment trasy obsahujici seznam bodu
  */

#ifndef GPSSEGMENT_H
#define GPSSEGMENT_H

#include <QObject>
#include <QList>
#include "gpspoint.h"

class GpsSegment : public QList<GpsPoint *>
{
    //Q_OBJECT
public:
    explicit GpsSegment();
    uint startTime();
    uint endTime();
    double startLat();
    double startLon();
    double endLat();
    double endLon();

public slots:

};

#endif // GPSSEGMENT_H
