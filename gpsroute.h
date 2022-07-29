/** @file gpsroute.h
  * Hlavickovy soubor tridy GpsRoute reprezentujici trasu
  */

#ifndef GPSROUTE_H
#define GPSROUTE_H

#include "gpspoint.h"
#include <QObject>
#include <QWidget>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "imageinfo.h"
#include <QAction>
#include <QColorDialog>
#include <QSpinBox>
#include <QToolButton>
#include <QComboBox>
#include <math.h>
#include "gpssegment.h"
#include <algorithm>
#include <iostream>


class GpsRoute : public QObject , public QTreeWidgetItem{
    Q_OBJECT
    QFile *data;
    QTextStream *stream;
    QXmlStreamReader *xml;

    double countDistanceGps(double posLeft, double posRight, double distProc);
    QString gpsInCurrentFormat(double lat, double lon);
    FormatHandler *formatHandler;
public:
    GpsRoute(int iid);
    ~GpsRoute();
    void approximatePosition(ImageInfo *image, GpsPoint *first, GpsPoint *second,
                             double offset, int method, GpsPoint *before, GpsPoint *after);
    int id;
    QColor *routeColor;
    QList<GpsSegment *> segmentList;
    bool loadFile(QString fileName);
    bool setGpsInImage(ImageInfo *image, double offset, uint maxDistM,
                       uint maxDistTime, int method, bool isJoinSeg);
    bool imageInSegment(ImageInfo *image,  double offset, GpsSegment *segment,
                        int method, GpsSegment *segBefore, GpsSegment *segAfter);
    void fillLabels();
    double countDistance(double lat1, double lon1, double lat2, double lon2);

    double minLat;
    double maxLat;
    double minLon;
    double maxLon;
    double routeLenght;

    qint64 startTime();
    qint64 endTime();
    double startLat();
    double startLon();
    double endLat();
    double endLon();
    QString name;
    QWidget *buttonWidget;
    QToolButton *bColor;
    void createLabels();
public slots:
    void retranslateUi();
    void setGpsFormat(QAction*action);
    void setDateTimeFormat(QAction*action);
    void sliderMoved(int value);
    void spinBoxChanged(int value);
    void changeColor();
    void setThread(QThread *newThread);
signals:
    void changeOpacity(int id, int value);
    void lineWidthChanged(int id, int value);
    void changeRouteColor(int id, QString color);
    void deleteRoute(int id);


};

#endif // GPSROUTE_H
