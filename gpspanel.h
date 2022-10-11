/** @file gpspanel.h
 * Hlavickovy soubor tridy GpsPanel dedici ze trify QDockWidget pro zobrazeni otevrenych GPS tras
 */

#ifndef GPSPANEL_H
#define GPSPANEL_H

#include "gpsroute.h"
#include <QDockWidget>
#include <QLayout>
#include <QToolButton>
#include <QTreeWidget>

class GpsPanel : public QDockWidget {
    Q_OBJECT
    QSize sizeHint() const;
    QTreeWidget* treeW;

public:
    GpsPanel(QWidget* parent = 0);
    void addGpsWidget(GpsRoute* gpsRouteWidget);
public slots:
    void retranslateUi();
};

#endif // GPSPANEL_H
