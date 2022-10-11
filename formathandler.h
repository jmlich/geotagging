/** @file formathandler.h
 * Hlavickovy soubor tridy FormatHandler dedici ze trify QObject pro prevadeni gps souradnic a casu
 * do pozadovaneho formatu
 */

#ifndef FORMATHANDLER_H
#define FORMATHANDLER_H

#include <QObject>
#include <math.h>
class FormatHandler : public QObject {
    Q_OBJECT

public:
    FormatHandler();
    QString formatGps;
    QString formatDateTime;
    QString convertDegreeToDM(double l);
    QString convertDegreeToDMS(double l);
    QString gpsInFormat(double l);
    QString gpsAllInFormat(double lat, double lon);
};

#endif // FORMATHANDLER_H
