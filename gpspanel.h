/** @file gpspanel.h
  * Hlavickovy soubor tridy GpsPanel dedici ze trify QDockWidget pro zobrazeni otevrenych GPS tras
  */

#ifndef GPSPANEL_H
#define GPSPANEL_H

#include <QTreeWidget>
#include <QDockWidget>
#include <QToolButton>
#include <QLayout>
#include "gpsroute.h"

class GpsPanel : public QDockWidget {
    Q_OBJECT
    QSize sizeHint() const;
    QTreeWidget *treeW;
public:
    GpsPanel( QWidget * parent = 0);
    void addGpsWidget(GpsRoute *gpsRouteWidget);
public slots:
    void retranslateUi();
};

#endif // GPSPANEL_H
