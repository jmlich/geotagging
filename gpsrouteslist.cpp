/** @file gpsrouteslist.cpp
 * Soubor s tridou GpsRoutesList dedici ze trid QObject a QList<GpsRoute *> obsahujici seznam tras
 */

#include "gpsrouteslist.h"

GpsRoutesList::GpsRoutesList()
    : QObject(0)
{
}

/**
 *zaradim trasu do seznamu setrideneho podle casu startu
 */
void GpsRoutesList::addRoute(GpsRoute* route)
{
    if (length() < 1) {
        append(route);
        return;
    }
    uint start = route->startTime(); // pocatecni bod trasy
    for (int i = 0; i < length(); i++) { // prochazim seznam tras
        if (start < at(i)->startTime()) { // vlozim trasu do seznamu pred prvni trasu s vyssim startovnim casem
            insert(i, route);
            break;
        } else if (i == length() - 1) { // jsem na konci seznamu
            insert(i + 1, route);
            break;
        }
    }
}

int GpsRoutesList::setGpsInImage(ImageInfo* image, double offset, QList<int> routesCheckedList,
    uint maxDistM, uint maxDistTime, int method, bool isJoinSeg)
{
    if (image->imageData->dateTime->isNull()) {
        return 0;
    }
    QDateTime time = image->imageData->dateTime->addSecs(offset * 3600);
    qint64 time_t = time.toSecsSinceEpoch();

    int lastI = -1;
    for (int i = 0; i < length(); i++) { // prochazim seznam tras
        if (!routesCheckedList.at(i)) { // trasa neni zaskrtnuta
            continue;
        }
        lastI = i;
        if (time_t < at(i)->startTime()) { //////////   //fotka ma nizsi cas nez trasa

            GpsPoint* secondPoint = at(i)->segmentList.first()->first();
            // najdu predposledni zaskrtlou trasu z dosud proslych
            int j;
            GpsPoint* firstPoint = NULL;
            for (j = i - 1; j >= 0; j--) {
                if (routesCheckedList.at(j)) {
                    firstPoint = at(j)->segmentList.last()->last();
                    // uprostred
                    qint64 firstTime = firstPoint->dateTime->toSecsSinceEpoch();
                    qint64 secondTime = secondPoint->dateTime->toSecsSinceEpoch();

                    // priradim nejblizsimu bodu
                    image->setCandidateIsCorrect(0);
                    at(i)->approximatePosition(image, firstPoint, secondPoint, offset, 0, NULL, NULL);
                    if (time_t - firstTime < secondTime - time_t) { // fotka ma bliz k prvnimu
                        image->setCandidateRouteName(QFileInfo(at(j)->name).fileName());
                    } else {
                        image->setCandidateRouteName(QFileInfo(at(i)->name).fileName());
                    }

                    return 0;
                }
            }
            if (j < 0) { // nenasla sem zaskrtlou trasu
                // pred
                image->setCandidateIsCorrect(0);
                image->setGpsCandidates(secondPoint->latitude,
                    secondPoint->longitude,
                    secondPoint->ele,
                    secondPoint->dateTime->addSecs(-offset * 3600),
                    0);
                image->setCandidateRouteName(QFileInfo(at(i)->name).fileName());
            }
            return 0;
        }
        if (time_t < at(i)->endTime()) { // fotka je v rozsahu jedne trasy -> zacnu prochazet jeji segmenty
            at(i)->setGpsInImage(image, offset, maxDistM, maxDistTime, method, isJoinSeg);
            image->setCandidateRouteName(QFileInfo(at(i)->name).fileName());
            return 0;
        }
    }
    // cas fotky je vetsi nez vsech tras
    if (lastI < 0) {
        return -1;
    }
    image->setCandidateIsCorrect(0);
    image->setGpsCandidates(at(lastI)->segmentList.last()->last()->latitude,
        at(lastI)->segmentList.last()->last()->longitude,
        at(lastI)->segmentList.last()->last()->ele,
        at(lastI)->segmentList.last()->last()->dateTime->addSecs(-offset * 3600),
        0);
    image->setCandidateRouteName(QFileInfo(at(lastI)->name).fileName());
    return 0;
}

void GpsRoutesList::deleteRoute(int id)
{
    for (int i = 0; i < this->length(); i++) {
        if (id == at(i)->id) {
            removeAt(i);
        }
    }
}
