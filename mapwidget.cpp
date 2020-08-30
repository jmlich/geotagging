/** @file mapwidget.cpp
  * Soubor s tridou MapWidget dedici ze tridy QDockWidget,
  * implementuje dokovaci okno s mapou
  */
#include <QtWebChannel>
#include "mapwidget.h"

MapWidget::MapWidget(  QWidget * parent) :
        QDockWidget(parent)
{
    setWindowTitle(tr("Map"));

    mapView = new QWebEngineView(parent);
    markersVisible = 1;
    routesVisible = 1;
    joinSegmentsVisible = 0;
    reliefVisible = 0;
    loadIsFinished = 0;
    connect(mapView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    mapView->load(QUrl("qrc:///leaflet.html"));

    QWebChannel *channel = new QWebChannel(mapView->page());
    channel->registerObject("mapWidget", this);
    mapView->page()->setWebChannel(channel);


    setAcceptDrops(false);
    mapView->setAcceptDrops(false);
    setFeatures(QDockWidget::DockWidgetClosable |
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable);
    newMarkerCandidade = 0;
    setWidgets();
}

void MapWidget::loadFinished(bool n) {
    Q_UNUSED(n)

    loadIsFinished = 1;
    if(!scriptsToRun.isEmpty())
    {
        foreach(QString script, scriptsToRun)
            mapView->page()->runJavaScript(script, [](const QVariant &result){ qDebug() << result.toString(); });
    }
    scriptsToRun.clear();
    disconnect(mapView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));

}

void MapWidget::retranslateUi() {

    mapSelect->setItemText(0, tr("Cyclo-tourist"));
    mapSelect->setItemText(1, tr("Google Roadmap"));
    mapSelect->setItemText(2, tr("Google Terrain"));
    mapSelect->setItemText(3, tr("Google Satellite"));
    mapSelect->setItemText(4, tr("Google Hybrid"));
    mapSelect->setItemText(5, tr("OSM Mapnik"));
    mapSelect->setItemText(6, tr("OSM Cyclomap"));

    bCenter->setToolTip(tr("Center map to view all"));
    bMarkersVisibility->setToolTip(tr("Show all markers"));
    bRoutesVisibility->setToolTip(tr("Show routes"));
    bJoinSegments->setToolTip(tr("Join segments"));
    bRelief->setToolTip(tr("Show relief"));

    setWindowTitle(tr("Map"));

}
void MapWidget::changeRouteOpacity(int id, int value) {
    QString scriptStr = QString("changeRouteOpacity(%1, %2);").arg(id).arg(value/10.0);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::lineWidthChanged(int id, int value) {
    QString scriptStr = QString("lineWidthChanged(%1, %2);").arg(id).arg(value);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::changeRouteColor(int id, QString color) {
    QString scriptStr = QString("changeRouteColor(%1, \"%2\");").arg(id).arg(color);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::settingNewMarker(QCursor cursor, QList<int> idList) {
    this->setCursor(cursor);
    QString ids = "[";
    int i;
    foreach(i, idList) {
        ids += QString("%1").arg(i) + ",";
    }
    ids.chop(1);
    ids += "]";

    QString scriptStr = QString("settingNewMarker(%1);").arg(ids);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::endSettingNewMarker(QCursor cursor) {
    this->setCursor(cursor);
    QString scriptStr = "endSettingNewMarker();";
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });

}

void MapWidget::newMarkerAdded(int id, double lat, double lon, double ele) {

    qDebug() << "newMarkerAdded" << id << lat << lon << ele;

    emit(settingNewMarkerFinished());
    emit(setGpsInImage(id, lat, lon, ele));
}
void MapWidget::setWidgets() {
    iconMarkerVisible = new QIcon(":/icons/markerShowR.png");
    iconRouteVisible = new QIcon(":/icons/chodecMapa.png");
    iconRelief = new QIcon(":/icons/relief.png");
    iconJoinSegments = new QIcon(":/icons/routeFull.png");

    QSize iconSize(35,35);
    QGridLayout *gl = new QGridLayout;
    int wCount = 0;
    mapSelect = new QComboBox;

    mapSelect->addItem(tr("Cyclo-tourist"), "Cykloturist");
    mapSelect->addItem(tr("Google Roadmap"), "ROADMAP");
    mapSelect->addItem(tr("Google Terrain"), "TERRAIN");
    mapSelect->addItem(tr("Google Satellite"), "SATELLITE");
    mapSelect->addItem( tr("Google Hybrid"), "HYBRID");
    mapSelect->addItem(tr("OSM Mapnik"), "OSMMapnik");
    mapSelect->addItem(tr("OSM Cyclomap"), "OSMCyklo");
    mapSelect->setCurrentIndex(5);
    mapSelect->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
    mapSelect->setFixedHeight(iconSize.height());

    connect(mapSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMap(int)));

    gl->addWidget(mapSelect,0,wCount);
    wCount++;

    bCenter = new QToolButton;
    bCenter->setMaximumSize(iconSize);
    bCenter->setIcon(QIcon(":/icons/terc.png"));
    bCenter->setIconSize(bCenter->size());
    bCenter->setToolTip(tr("Center map to view all"));
    connect(bCenter, SIGNAL( pressed () ), this, SLOT (centerMap() ) );
    gl->addWidget(bCenter,0,wCount);

    QPalette pal = bCenter->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    bCenter->setPalette(pal);
    bCenter->setBackgroundRole(QPalette::Window);
    wCount++;


    bMarkersVisibility = new QToolButton;
    bMarkersVisibility->setMaximumSize(iconSize);
    bMarkersVisibility->setIcon(*iconMarkerVisible);
    bMarkersVisibility->setIconSize(bMarkersVisibility->size());
    bMarkersVisibility->setToolTip(tr("Show all markers"));
    cMarkersVisibility = new QCheckBox(bMarkersVisibility);
    cMarkersVisibility->setChecked(markersVisible);
    cMarkersVisibility->setMaximumSize(QSize(15,20));

    bMarkersVisibility->setContentsMargins(QMargins(0,0,0,0));
    connect(cMarkersVisibility, SIGNAL(clicked()), this, SLOT(setMarkersVisibility()));
    connect(bMarkersVisibility, SIGNAL(clicked()), this, SLOT(setMarkersVisibility()) );
    connect(bMarkersVisibility, SIGNAL(clicked()), cMarkersVisibility, SLOT(toggle()) );
    gl->addWidget(bMarkersVisibility,0,wCount);
    wCount++;

    bRoutesVisibility = new QToolButton;
    bRoutesVisibility->setMaximumSize(iconSize);
    bRoutesVisibility->setIcon(*iconRouteVisible);
    bRoutesVisibility->setIconSize(bRoutesVisibility->size());
    bRoutesVisibility->setToolTip(tr("Show routes"));
    cRoutesVisibility = new QCheckBox(bRoutesVisibility);
    cRoutesVisibility->setChecked(routesVisible);
    cRoutesVisibility->setMaximumSize(QSize(15,20));
    bRoutesVisibility->setContentsMargins(QMargins(0,0,0,0));;

    connect(cRoutesVisibility, SIGNAL(clicked()), this, SLOT(setRoutesVisibility()));
    connect(bRoutesVisibility, SIGNAL(clicked()), this, SLOT(setRoutesVisibility()) );
    connect(bRoutesVisibility, SIGNAL(clicked()), cRoutesVisibility, SLOT(toggle()) );
    gl->addWidget(bRoutesVisibility,0,wCount);
    wCount++;

    bJoinSegments = new QToolButton;
    bJoinSegments->setMaximumSize(iconSize);
    bJoinSegments->setIcon(*iconJoinSegments);
    bJoinSegments->setIconSize(bJoinSegments->size());
    bJoinSegments->setToolTip(tr("Join segments"));

    cJoinSegments = new QCheckBox(bJoinSegments);
    cJoinSegments->setChecked(joinSegmentsVisible);
    cJoinSegments->setMaximumSize(QSize(15,20));
    bJoinSegments->setContentsMargins(QMargins(0,0,0,0));;
    connect(cJoinSegments, SIGNAL(clicked()), this, SLOT(setJoinSegments()));
    connect(bJoinSegments, SIGNAL(clicked()), this, SLOT(setJoinSegments()) );
    connect(bJoinSegments, SIGNAL(clicked()), cJoinSegments, SLOT(toggle()) );

    gl->addWidget(bJoinSegments,0,wCount);
    wCount++;

    bRelief = new QToolButton;
    bRelief->setMaximumSize(iconSize);
    bRelief->setIcon(*iconRelief);
    bRelief->setIconSize(bRelief->size());
    bRelief->setToolTip(tr("Show relief"));
    cRelief = new QCheckBox(bRelief);
    cRelief->setChecked(reliefVisible);
    cRelief->setMaximumSize(QSize(15,20));
    bJoinSegments->setContentsMargins(QMargins(0,0,0,0));;
    connect(cRelief, SIGNAL(clicked()), this, SLOT(flipRelief()));
    connect(bRelief, SIGNAL(clicked()), this, SLOT(flipRelief()) );
    connect(bRelief, SIGNAL(clicked()), cRelief, SLOT(toggle()) );

    gl->addWidget(bRelief,0,wCount);
    wCount++;

    gl->addWidget(mapView,1,0,1,wCount+1);
    gl->setContentsMargins(QMargins(0,0,0,0));
    QWidget *w = new QWidget;
    w->setLayout(gl);
    setWidget(w);
}

void MapWidget::flipRelief() {
    reliefVisible = !reliefVisible;
    QStringList scriptStr;
    scriptStr << QString("flipRelief(%1);").arg(reliefVisible);
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });

}

void MapWidget::setJoinSegments() {
    joinSegmentsVisible = !joinSegmentsVisible;
    QStringList scriptStr;
    scriptStr << QString("setJoinSegments(%1);").arg(joinSegmentsVisible && routesVisible);
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::changeMap(int mapI) {
    QString s = mapSelect->itemData(mapI).toString();
    QStringList scriptStr;
    scriptStr << QString("setMapType(\"%1\");").arg(s);
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::addMarker(int id, double lat, double lon) {
    QStringList scriptStr;
    scriptStr << QString("addMarker(%1, %2, %3, %4); centerInBounds(1,0);")
                 .arg(QString::number(lat, 'f', 10))
                 .arg(QString::number(lon, 'f', 10))
                 .arg(id)
                 .arg(markersVisible);

    if(!loadIsFinished) {
         scriptsToRun << scriptStr.join("\n");
    } else {
        mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
    }
}

void MapWidget::markerSelected(int id, bool isSelected) {
    QStringList scriptStr;
    scriptStr <<"for (i in markers) {"
            <<    QString("if(%1 == -1 || %1==markers[i].options.id){").arg(id)
            <<    QString("markerSelected(%1, i, %2);").arg(isSelected).arg(markersVisible)
            <<  " }"
            << " }";
//    qDebug() << scriptStr;
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::markerClicked(int id) {
    QStringList scriptStr;
    scriptStr << QString("markerClicked(%1, %2);").arg(id).arg((QApplication::keyboardModifiers() & Qt::ControlModifier));

    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
    emit(mClicked(id, 0, 1));
}

void MapWidget::markerDragged(int id) {
    qDebug() << "markerDragged" << id;
    idDragged = id;
    QMessageBox *mb = new QMessageBox(QMessageBox::Question, tr("Change picture location"),
                                      tr("Change picture coordinates to the new position?"),
                                      QMessageBox::Yes | QMessageBox::No);

    QPalette pal = mb->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    mb->setPalette(pal);
    int ret = mb->exec();
    if (ret == QMessageBox::Yes) {
        setNewGpsInImage();
    } else {
        setMarkerLastPosition();
    }
}
void MapWidget::setNewGpsInImage() {
    qDebug() << "setNewGpsInImage" << idDragged;
    QString scriptStr = QString("setNewMarkerPosition(%1);").arg(idDragged);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });

//    mapView->page()->mainFrame()->evaluateJavaScript( scriptStr).toList();
}

void MapWidget::setMarkerLastPosition() {
    QString scriptStr = QString("setOldMarkerPosition(%1);").arg(idDragged);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });

}

void MapWidget::addRoute(GpsRoute *route) {

    for (int i=0; i<route->segmentList.length(); i++) {
        QString points = "";
        GpsSegment *seg = route->segmentList.at(i);
        for (int j=0; j<seg->length(); j++) {
            points.append(QString("[%1, %2],\n")
                          .arg(QString::number(seg->at(j)->latitude, 'f', 10))
                          .arg(QString::number(seg->at(j)->longitude, 'f', 10)));
        }
        QStringList scriptStr;
        scriptStr << "var routeCoordinatesList = ["
                << points
                << "];"
                << QString("addRoute(routeCoordinatesList, %1, %2, \"%3\", 1);")
                .arg(route->id).arg(routesVisible).arg(route->routeColor->name());

        if (i+1 < route->segmentList.length()) { //pridani spojeni segmentu
            scriptStr << "var routeCoordinatesList2 = ["
                    << QString("[%1, %2],").arg(QString::number(route->segmentList.at(i)->last()->latitude, 'f', 10))
                            .arg(QString::number(route->segmentList.at(i)->last()->longitude, 'f', 10))
                    << QString("[%1, %2]").arg(QString::number(route->segmentList.at(i+1)->first()->latitude, 'f', 10))
                            .arg(QString::number(route->segmentList.at(i+1)->first()->longitude, 'f', 10))
                    << "];"
                    << QString("addRoute(routeCoordinatesList2, %1, %2, \"%3\",0);").arg(route->id).arg(routesVisible && joinSegmentsVisible).arg(route->routeColor->name());

        }

        if (!loadIsFinished) {
            scriptsToRun << scriptStr.join("\n");
        } else {
            mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
        }
    }
}

void MapWidget::centerMap() {
    QStringList scriptStr;
    scriptStr << "centerInBounds(1,1)";
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::setMarkersVisibility() {
    QStringList scriptStr;
    markersVisible = !markersVisible;
    scriptStr << QString("setMarkersVisibility(%1);").arg(markersVisible);
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}

void MapWidget::setRoutesVisibility() {
    routesVisible = !routesVisible;
    QStringList scriptStr;
    scriptStr << QString("setRoutesVisibility(%1);").arg(routesVisible)
         << QString("setJoinSegments(%1);").arg(joinSegmentsVisible && routesVisible);

    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}


void MapWidget::deleteMarker(int id) {
    QStringList scriptStr;
    scriptStr << QString("deleteMarker(%1);").arg(id);
    mapView->page()->runJavaScript(scriptStr.join("\n"), [](const QVariant &result){ qDebug() << result.toString(); });
}
void MapWidget::keyPressEvent(QKeyEvent *event) {
    emit(processEvent(event));
}

void MapWidget::deleteRoute(int id) {
    QString scriptStr = QString("deleteRoute(%1);").arg(id);
    mapView->page()->runJavaScript(scriptStr, [](const QVariant &result){ qDebug() << result.toString(); });
}
