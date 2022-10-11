/** @file gpsrouteslist.h
 * Hlavickovy soubor tridy GpsRouteList dedici ze trid QObject a QList<GpsRoute *> obsahujici seznam tras
 */

#ifndef GPSROUTESLIST_H
#define GPSROUTESLIST_H
#include "gpsroute.h"
#include "imageinfo.h"
#include <QList>
#include <QObject>

class GpsRoutesList : public QObject, public QList<GpsRoute*> {
public:
    GpsRoutesList();
    void addRoute(GpsRoute* route);
    int setGpsInImage(ImageInfo* image, double offset, QList<int> routesCheckedList,
        uint maxDistM, uint maxDistTime, int method, bool isJoinSeg);
    void deleteRoute(int id);
};

#endif // GPSROUTESLIST_H
