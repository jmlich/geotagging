/** @file gpssegment.h
 * Hlavickovy soubor tridy GpsSegment dedici ze tridy QList<GpsPoint *>
 * reprezentujici segment trasy obsahujici seznam bodu
 */

#ifndef GPSSEGMENT_H
#define GPSSEGMENT_H

#include "gpspoint.h"
#include <QList>
#include <QObject>

class GpsSegment : public QList<GpsPoint*> {
    // Q_OBJECT
public:
    explicit GpsSegment();
    qint64 startTime();
    qint64 endTime();
    double startLat();
    double startLon();
    double endLat();
    double endLon();

public slots:
};

#endif // GPSSEGMENT_H
