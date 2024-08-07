/** @file mapwidget.h
 * Hlavickovy soubor tridy MapWidget dedici ze tridy QDockWidget,
 * implementuje dokovaci okno s mapou
 */
#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "gpsroute.h"
#include "gpsrouteslist.h"
#include "keyeventhandler.h"
#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QPushButton>
#include <QToolButton>
#include <QWebEngineHistory>
#include <QWebEngineHistoryItem>
#include <QWebEnginePage>
#include <QWebEngineView>
class MapWidget : public QDockWidget {
    Q_OBJECT
    QToolButton* bCenter;
    QToolButton* bMarkersVisibility;
    QToolButton* bObjectsVisibility;
    QToolButton* bRoutesVisibility;
    QToolButton* bJoinSegments;
    QToolButton* bRelief;

    QCheckBox* cMarkersVisibility;
    QCheckBox* cObjectsVisibility;
    QCheckBox* cRoutesVisibility;
    QCheckBox* cJoinSegments;
    QCheckBox* cRelief;

    QComboBox* mapSelect;
    QIcon* iconMarkerVisible;
    QIcon* iconObjectVisible;
    // QIcon *iconMarkerHidden;
    QIcon* iconRouteVisible;
    // QIcon *iconRouteHidden;
    QIcon* iconJoinSegments;
    // QIcon *iconHideSegments;
    QIcon* iconRelief;
    int idDragged;

    void keyPressEvent(QKeyEvent* event);
    void setWidgets();
    QStringList scriptsToRun;

public:
    explicit MapWidget(QWidget* parent = 0);
    QUrl* url;
    QWebEngineView* mapView;
    QWebEnginePage* mapPage;
    bool markersVisible;
    bool objectsVisible;
    bool routesVisible;
    bool newMarkerCandidade;
    bool joinSegmentsVisible;
    bool reliefVisible;

    bool loadIsFinished;
    void settingNewMarker(QCursor cursor, QList<int> idList);
    void settingNewObjectMarker(QCursor cursor, QList<int> idList);
    void settingRemoveCameraMarker(QList<int> idList);
    void settingRemoveObjectMarker(QList<int> idList);
    void endSettingNewMarker(QCursor cursor);
    void endSettingNewObjectMarker(QCursor cursor);
    GpsRoutesList* gpsRoutesList;

signals:
    void mClicked(int id, bool clickMarker, bool focus);
    void oClicked(int id, bool clickMarker, bool focus);
    void processEvent(QKeyEvent* event);
    void setGpsInImage(int id, double lat, double lon, double ele);
    void setObjectGpsInImage(int id, double lat, double lon, double ele);
    void setCameraDirectionInImage(int id, double direction, double angleOfView);
    void settingNewMarkerFinished();

public slots:
    void addObjectMarker(int id, double lat, double lon);
    void addCameraMarker(int id, double lat, double lon, double direction, double angleOfView);
    void addRoute(GpsRoute* route);
    void markerSelected(int id, bool s);
    void markerClicked(int id);
    void markerDragged(int id);

    void objectSelected(int id, bool s);
    void objectClicked(int id);
    void objectDragged(int id);

    void centerMap();
    void setMarkersVisibility();
    void setObjectsVisibility();
    // void setMarkersVisibilityC();
    void setRoutesVisibility();
    // void setRoutesVisibilityC();
    void deleteMarker(int id);
    void changeMap(int mapI);
    void setNewGpsInImage();
    void setMarkerLastPosition();
    void setNewObjectPositionIntoImage();
    void setObjectMarkerLastPosition();
    void newCameraMarkerAdded(int id, double lat, double lon, double ele);
    void newObjectMarkerAdded(int id, double lat, double lon, double ele);
    void directionUpdated(int id, double direction, double angleOfView);
    void setttingCameraMarkerDirection(QList<int> idList, double direction);
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
