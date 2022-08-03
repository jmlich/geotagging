/** @file mapwidget.h
  * Hlavickovy soubor tridy MapWidget dedici ze tridy QDockWidget,
  * implementuje dokovaci okno s mapou
  */
#ifndef MAPWIDGET_H
#define MAPWIDGET_H


#include <QDockWidget>
#include <QWebEngineHistory>
#include <QWebEngineHistoryItem>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QToolButton>
#include <QPushButton>
#include <QComboBox>
#include <QApplication>
#include "keyeventhandler.h"
#include "gpsroute.h"
#include "gpsrouteslist.h"
class MapWidget : public QDockWidget
{
    Q_OBJECT
    QToolButton *bCenter;
    QToolButton *bMarkersVisibility;
    QToolButton *bRoutesVisibility;
    QToolButton *bJoinSegments;
    QToolButton *bRelief;

    QCheckBox *cMarkersVisibility;
    QCheckBox *cRoutesVisibility;
    QCheckBox *cJoinSegments;
    QCheckBox *cRelief;

    QComboBox *mapSelect;
    QIcon *iconMarkerVisible;
   // QIcon *iconMarkerHidden;
    QIcon *iconRouteVisible;
   // QIcon *iconRouteHidden;
    QIcon *iconJoinSegments;
   // QIcon *iconHideSegments;
    QIcon *iconRelief;
    int idDragged;

    void keyPressEvent ( QKeyEvent * event );
    void setWidgets();
    QStringList scriptsToRun;

public:
    explicit MapWidget(QWidget *parent = 0);
    QUrl *url;
    QWebEngineView *mapView;
    QWebEnginePage *mapPage;
    bool markersVisible;
    bool routesVisible;
    bool newMarkerCandidade;
    bool joinSegmentsVisible;
    bool reliefVisible;

    bool loadIsFinished;
    void settingNewMarker(QCursor cursor, QList<int> idList);
    void endSettingNewMarker(QCursor cursor);
    GpsRoutesList *gpsRoutesList;


signals:
    void mClicked(int id, bool clickMarker, bool focus);
    void processEvent(QKeyEvent *event);
    void setGpsInImage(int id, double lat, double lon, double ele);
    void settingNewMarkerFinished();

public slots:
    void addObjectMarker(int id, double lat, double lon);
    void addMarker(int id, double lat, double lon);
    void addRoute(GpsRoute *route);
    void markerSelected(int id, bool s);
    void markerClicked(int id);
    void markerDragged(int id);

    void objectSelected(int id, bool s);
    void objectClicked(int id);
    void objectDragged(int id);


    void centerMap();
    void setMarkersVisibility();
    //void setMarkersVisibilityC();
    void setRoutesVisibility();
    //void setRoutesVisibilityC();
    void deleteMarker(int id);
    void changeMap(int mapI);
    void setNewGpsInImage();
    void setMarkerLastPosition();
    void setObjectMarkerLastPosition();
    void newMarkerAdded(int id, double lat, double lon, double ele);
    void retranslateUi();
    void changeRouteOpacity(int id, int value);
    void lineWidthChanged(int id, int value);
    void changeRouteColor(int id, QString color);
    void deleteRoute(int id);
    void setJoinSegments();
    void flipRelief();
    void loadFinished(bool n);

   // void changeEvent(QEvent *e);
};

#endif // MAPWIDGET_H
