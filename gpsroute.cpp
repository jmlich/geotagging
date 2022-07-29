/** @file gpsroute.h
  * Soubor s tridou GpsRoute reprezentujici trasu
  */

#define _USE_MATH_DEFINES
#include <cmath>
#include "gpsroute.h"

GpsRoute::GpsRoute(int iid) :
    QObject(), QTreeWidgetItem()
{
    xml = new QXmlStreamReader;
    minLat = 1000;
    maxLat = 1000;
    minLon = 1000;
    maxLon = 1000;
    routeLenght = 0;

    id = iid;
    formatHandler = new FormatHandler;

}

GpsRoute::~GpsRoute() {
    emit deleteRoute(id);
}

void GpsRoute::setThread(QThread *newThread) {
    this->moveToThread(newThread);
}

void GpsRoute::createLabels() {
    QStringList colorNames;
    colorNames <<"red"<<"green"<<"blue"<<"cyan"<<"magenta"<<"yellow"<<"darkRed"<<"darkBlue"<<"darkCyan"<<"darkMagenta";

    QSize iconSize(26,26);

    QPixmap pixmap(20,20);

    ////////////////////////////////////
    bColor = new QToolButton;
    bColor->setMaximumSize(iconSize);
    //QColor::colorNames()
    routeColor = new QColor(colorNames.at(id % colorNames.count()));
    pixmap.fill(*routeColor);
    bColor->setIcon(QIcon(pixmap));
    bColor->setIconSize(bColor->size());
    bColor->setToolTip(tr("line color"));
    connect(bColor, SIGNAL(clicked()), this, SLOT(changeColor()));
    ////////////////////////////////////////////

    QSpinBox *spinBoxWidth = new QSpinBox;
    spinBoxWidth->setRange(1,10);
    spinBoxWidth->setValue(3);
    spinBoxWidth->setToolTip(tr("line width"));
    connect(spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(spinBoxChanged(int)));

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(100);
    slider->setRange(0,10);
    slider->setValue(slider->maximum());
    slider->setToolTip(tr("line opacity"));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));

    QToolButton *bClose = new QToolButton;
    bClose->setMaximumSize(iconSize);
    bClose->setIcon(QIcon(":/icons/closeX.png"));
    bClose->setIconSize(bClose->size());
    bClose->setToolTip(tr("close"));
    connect(bClose, SIGNAL(clicked()), this, SLOT(deleteLater()));

    buttonWidget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    this->buttonWidget->setLayout(layout);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignLeft);
    layout->addWidget(bColor);
    layout->addWidget(spinBoxWidth);
    layout->addWidget(slider);
    layout->addStretch();
    layout->addWidget(bClose);
}


void GpsRoute::changeColor() {
    QColorDialog cd;
    QColor c = cd.getColor(*routeColor);
    if(c.isValid()) {
        *routeColor = c;
    }
    QPixmap pixmap(20,20);
    pixmap.fill(*routeColor);
    bColor->setIcon(QIcon(pixmap));
    emit changeRouteColor(id, routeColor->name());
}

void GpsRoute::sliderMoved(int value) {
    emit changeOpacity(id,value);
}

void GpsRoute::spinBoxChanged(int value) {
    emit lineWidthChanged(id,value);
}

void GpsRoute::retranslateUi() {

    this->child(0)->setText(0,tr("File path"));
    this->child(1)->setText(0,tr("Start time"));
    this->child(2)->setText(0,tr("End time"));

    this->child(3)->setText(0,tr("Start point"));
    double lat = segmentList.first()->first()->latitude;
    double lon = segmentList.first()->first()->longitude;
    this->child(3)->setText(1,formatHandler->gpsAllInFormat(lat,lon));


    this->child(4)->setText(0,tr("End point"));
    lat = segmentList.last()->last()->latitude;
    lon = segmentList.last()->last()->longitude;
    this->child(4)->setText(1,formatHandler->gpsAllInFormat(lat,lon));

    this->child(5)->setText(0,tr("Total time"));
    qulonglong sec = segmentList.last()->last()->dateTime->toSecsSinceEpoch() - segmentList.first()->first()->dateTime->toSecsSinceEpoch();
    QString days;
    days.setNum(sec / (24*60*60));
    sec = sec % (24*60*60);
    QString hours;
    hours.setNum(sec/3600);
    sec = sec % 3600;
    QString minutes;
    minutes.setNum(sec/60);
    sec = sec % 60;
    QString seconds;
    seconds.setNum(sec);
    this->child(5)->setText(1,((days.toInt() > 0)?(days + tr("Days ")):"") + hours + ":" + minutes + ":" + seconds);

    this->child(6)->setText(0,tr("Route length"));

    double lenght = round(routeLenght) / 1000;
    QString rl = QString(tr("%1 kilometers")).arg(lenght);
    this->child(6)->setText(1,rl);
}

double GpsRoute::countDistanceGps(double posFirst, double posSecond, double distProc) {
    return posFirst * (1.0-distProc) + posSecond * distProc;
}

void GpsRoute::approximatePosition(ImageInfo *image ,GpsPoint *first, GpsPoint *second,
                                   double offset, int method, GpsPoint *before, GpsPoint *after)
{
    int imageTime = image->imageData->dateTime->addSecs(offset*3600).toSecsSinceEpoch();
    if(method == 0) { //nejblizsi bod
        if(imageTime-first->dateTime->toSecsSinceEpoch() < second->dateTime->toSecsSinceEpoch() - imageTime) { //blize k prvnimu
            image->setGpsCandidates(first->latitude,
                                    first->longitude,
                                    first->ele,
                                    first->dateTime->addSecs(-offset*3600),
                                    method);
        } else {
            image->setGpsCandidates(second->latitude,
                                    second->longitude,
                                    second->ele,
                                    second->dateTime->addSecs(-offset*3600),
                                    method);
        }
    } else if(method == 1) { //ze dvou okolnich bodu
        int timeDist = second->dateTime->toSecsSinceEpoch() - first->dateTime->toSecsSinceEpoch();
        int timeDistPoint = imageTime - first->dateTime->toSecsSinceEpoch();
        double distProc = static_cast<double>(timeDistPoint) / static_cast<double>(timeDist);
        double lat = countDistanceGps(first->latitude, second->latitude, distProc);
        double lon = countDistanceGps(first->longitude, second->longitude, distProc);
        double alt;
        if(first->ele < -999 || second->ele < -999) {
            alt = -1000;
        } else {
            alt = countDistanceGps(first->ele, second->ele, distProc);
        }
        image->setGpsCandidates(lat,lon,alt, *image->imageData->dateTime, method);
    } else if(method == 2) {
        double v1tmp = -1;
        double v2tmp = -1;
        double t1 = first->dateTime->toSecsSinceEpoch();
        double t2 = second->dateTime->toSecsSinceEpoch();
        if (before != NULL) {
            uint deltaT1 = t1 - before->dateTime->toSecsSinceEpoch();
            double deltaS1 = countDistance(before->latitude,before->longitude, first->latitude, first->longitude);
            v1tmp = deltaS1 / deltaT1;
        } else {
            uint deltaT1 = t2 - t1;
            double deltaS1 = countDistance(second->latitude,second->longitude, first->latitude, first->longitude);
            v1tmp = deltaS1 / deltaT1;
        }

        if(after != NULL) {
            uint deltaT2 = after->dateTime->toSecsSinceEpoch() - t2;
            double deltaS2 = countDistance(second->latitude,second->longitude, after->latitude, after->longitude);
            v2tmp = deltaS2 / deltaT2;
        } else {
            uint deltaT2 = t2 - t1;
            double deltaS2 = countDistance(second->latitude,second->longitude, first->latitude, first->longitude);
            v2tmp = deltaS2 / deltaT2;
        }

        if(v1tmp == 0) {
            v1tmp = 0.00001;
        }

        double p = v2tmp/v1tmp;

        double deltaS = countDistance(first->latitude, first->longitude, second->latitude, second->longitude);
        uint deltaT = t2-t1;
        //////////
        if(deltaS==0) {
            deltaS = 0.00001;
        }
        //////////

        double v1 = (2*deltaS)/(deltaT * (1+p));
        double v2 = v1 * p;
        uint t = imageTime - first->dateTime->toSecsSinceEpoch();
        double a = (v2-v1)/(deltaT);
        double s = v1 * t + 0.5 * a * pow(t,2);

        double distProc = s / deltaS;
        double lat = countDistanceGps(first->latitude, second->latitude, distProc);
        double lon = countDistanceGps(first->longitude, second->longitude, distProc);
        double alt;
        if(first->ele < -999 || second->ele < -999) {
            alt = -1000;
        } else {
            alt = countDistanceGps(first->ele, second->ele, distProc);
        }
        image->setGpsCandidates(lat,lon,alt, *image->imageData->dateTime, method);
    }
}

bool GpsRoute::setGpsInImage(ImageInfo *image,  double offset, uint maxDistM, uint maxDistTime,int method, bool isJoinSeg) {
    QDateTime time = image->imageData->dateTime->addSecs(offset*3600);
    uint time_t = time.toSecsSinceEpoch();

    for(int i=0; i<segmentList.length(); i++) {
        if(time_t < segmentList.at(i)->startTime()) {
            GpsPoint *secondPoint = segmentList.at(i)->first();
            //najdu predposledni zaskrtlou trasu z dosud proslych
            GpsPoint *firstPoint = NULL;

            if (i > 0) {   //neni to prvni segment
                firstPoint = segmentList.at(i-1)->last();
                uint firstTime = firstPoint->dateTime->toSecsSinceEpoch();
                uint secondTime = secondPoint->dateTime->toSecsSinceEpoch();

                //testuju jestli muzu spojit segmenty
                if(isJoinSeg
                        && firstTime < secondTime
                        && secondTime - firstTime <= maxDistTime
                        && countDistance(secondPoint->latitude, secondPoint->longitude, firstPoint->latitude, firstPoint->longitude <= maxDistM))
                {
                    image->setCandidateIsCorrect(1);
                    GpsPoint *before = NULL;
                    GpsPoint *after = NULL;
                    if(segmentList.at(i-1)->length() > 1) {
                        before = segmentList.at(i-1)->at(segmentList.at(i-1)->length()-2);
                    }
                    if(segmentList.at(i)->length() > 1) {
                        after = segmentList.at(i)->at(1);
                    }
                    approximatePosition(image, firstPoint, secondPoint, offset, method, before, after);
                    image->setCandidateRouteName(QFileInfo(this->name).fileName());
                } else {
                    //segmenty nelze spojit
                    //priradim k nejblizsimu bodu
                    image->setCandidateIsCorrect(0);
                    approximatePosition(image, firstPoint, secondPoint, offset, 0, NULL, NULL);
                    image->setCandidateRouteName(QFileInfo(this->name).fileName());
                }
                return 0;
            }
        } else if(time_t <= segmentList.at(i)->endTime()) { //fotka uvnitr segmentu

            GpsSegment *segBefore = NULL;
            GpsSegment *segAfter = NULL;
            if(i>0) {
                segBefore = segmentList.at(i-1);
            }
            if(i+1 < segmentList.length()) {
                segAfter = segmentList.at(i+1);
            }

            imageInSegment(image, offset, segmentList.at(i), method, segBefore, segAfter);
            return 0;
        }
    }
    return 0;
}

bool GpsRoute::imageInSegment(ImageInfo *image,  double offset, GpsSegment *segment, int method, GpsSegment *segBefore, GpsSegment *segAfter) {
    QDateTime time = image->imageData->dateTime->addSecs(offset*3600);
    uint time_t = time.toSecsSinceEpoch();

    image->setCandidateIsCorrect(1);
    image->setCandidateRouteName(QFileInfo(this->name).fileName());
    //metoda puleni intervalu
    int k = 0;
    for(int i = 0, j = segment->count()-1; j-i > 0;) {
        k = (i+j)/2;

        if(time_t >= segment->at(k)->dateTime->toSecsSinceEpoch()) {
            if(time_t < segment->at(k + 1)->dateTime->toSecsSinceEpoch() ) { //nasla sem dva okolni body
                break;
            } else {
                i = k;
            }
        } else {
            j = k;
        }
    }

    GpsPoint *before = NULL;
    GpsPoint *after = NULL;
    if (k > 0) {   //neni to prvni bod segmentu
        before = segment->at(k-1);
    } else if(segBefore != NULL) {      //testovat spojeni segmentu !!!!!!!
        before = segBefore->last();
    }

    if(segment->length() > k+3) {
        after = segment->at(k+2);
    } else if(segAfter != NULL) {
        after = segAfter->first();
    }

    approximatePosition(image, segment->at(k), segment->at(k + 1), offset, method, before, after);

    return 0;
}

bool GpsRoute::loadFile(QString fileName) {
    name = fileName;
    data= new QFile(fileName);

    //otevre soubor
    if (!data->open ( QIODevice::ReadOnly)) { //otevreni souboru
        return 0;
    }

    xml->setDevice(data);
    xml->readNextStartElement();

    if (xml->name() == QStringLiteral("gpx")){  //je gpx soubor
        int state = 0;
        GpsPoint *point = NULL;
        GpsPoint *lastPoint = NULL;
        GpsSegment *currentSegment = NULL;

        while (!xml->atEnd()) {
            xml->readNext();

            switch (state) {
                case 0: //neni nacten zacatek segmentu
                    if (xml->isStartElement() &&  xml->name() == QStringLiteral("trkseg")) { //novy segment
                        currentSegment = new GpsSegment;
                        state = 1;


                    //else if (xml->atEnd())
                    //  break;
                    } else {
                        continue;
                    }
                break;
                case 1: //neni nacten bod
                    if (xml->isStartElement() && (xml->name() == QStringLiteral("trkpt"))) { //novy bod
                        lastPoint = point;
                        point = new GpsPoint;
                        point->longitude = xml->attributes().value("lon").toString().toDouble();
                        point->latitude = xml->attributes().value("lat").toString().toDouble();

                        if (maxLon == 1000 || point->longitude > maxLon) {
                            maxLon = point->longitude;
                        }
                        if (minLon == 1000 || point->longitude <minLon) {
                            minLon = point->longitude;
                        }
                        if (maxLat == 1000 || point->latitude > maxLat) {
                            maxLat = point->latitude;
                        }
                        if (minLon == 1000 || point->latitude <minLat) {
                            minLat = point->latitude;
                        }

                        state = 2;

                    } else if (xml->isEndElement() && xml->name() == QStringLiteral("trkseg")) {   //konec segmentu
                        state = 0;

                    //else if (xml->atEnd())
                    //  break;
                    } else {
                        continue;
                    }
                break;

                case 2: //je nacten bod ale ne cas
                    if (xml->isStartElement() && xml->name() == QStringLiteral("time")) {
                        state = 3;
                    } else if (xml->isStartElement() && xml->name() == QStringLiteral("ele")) {
                        state=4;
                    } else if (xml->isEndElement() && (xml->name() == QStringLiteral("trkpt"))) {
                        state = 1;
                    }
                break;
                case 3: //ctu obsah elementu time
                    if (xml->isCharacters()) {
                        point->setTime(xml->text().toString());

                        //delka trasy
                        if(lastPoint != NULL) {
                            double dist = countDistance( point->latitude, point->longitude, lastPoint->latitude, lastPoint->longitude);
                            routeLenght += dist;
                        } if(currentSegment->length() == 0) {//pripojuji prvni bod
                            segmentList.append(currentSegment);
                        }
                        currentSegment->append(point);

                    } else if (xml->isEndElement() && xml->name() == QStringLiteral("time")) {
                        state = 2;
                    }
                break;
                case 4:
                    if (xml->isCharacters()) {
                        point->ele = xml->text().toString().toDouble();
                    } else if (xml->isEndElement() && xml->name() == QStringLiteral("ele")) {
                        state = 2;
                    }
                break;
            }
        }
        data->close();

        if(segmentList.length() < 1) {
            return 1;
        }

    } else {
        data->close();
        return 1;
    }
    return 0;

}

//Great Circle distance algoritmus
double GpsRoute::countDistance(double lat1, double lon1, double lat2, double lon2) {
    double earthMeanRadius = 6371009; //v metrech

    double d2r = M_PI / 180.0;  //stupne na radiany
    double distLat = (lat2 - lat1) * d2r;
    double distLon = (lon2 - lon1) * d2r;

    double a = pow(sin(distLat/2.0), 2) + cos(lat1*d2r) * cos(lat2*d2r) * pow(sin(distLon/2.0), 2);
    //double c = 2 * asin(1<sqrt(a) ? 1 : sqrt(a));
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double distance = earthMeanRadius * c;
    return distance;
}

void GpsRoute::fillLabels() {
    this->setText(0,QFileInfo(name).fileName());
    QPixmap pixmapR(":/icons/chodec.png");
    pixmapR = pixmapR.scaled(20,20);
    this->setData(0,Qt::DecorationRole, pixmapR);
    ///////////////

    if(!segmentList.empty()) {
        QTreeWidgetItem *twtmp = new QTreeWidgetItem(this);

        twtmp->setText(0,tr("File path"));
        twtmp->setText(1,QFileInfo(name).canonicalFilePath());


        twtmp = new QTreeWidgetItem(this);
        twtmp->setText(0,tr("Start time"));
        twtmp->setText(1,segmentList.first()->first()->dateTime->toString(formatHandler->formatDateTime) +" UTC");


        twtmp = new QTreeWidgetItem(this);
        twtmp->setText(0,tr("End time"));
        twtmp->setText(1,segmentList.last()->last()->dateTime->toString(formatHandler->formatDateTime) +" UTC");


        twtmp = new QTreeWidgetItem(this);
        twtmp->setText(0,tr("Start point"));
        double lat = segmentList.first()->first()->latitude;
        double lon = segmentList.first()->first()->longitude;
        twtmp->setText(1,formatHandler->gpsAllInFormat(lat,lon));


        twtmp = new QTreeWidgetItem(this);
        twtmp->setText(0,tr("End point"));
        lat = segmentList.last()->last()->latitude;
        lon = segmentList.last()->last()->longitude;
        twtmp->setText(1,formatHandler->gpsAllInFormat(lat,lon));

        qulonglong sec = segmentList.last()->last()->dateTime->toSecsSinceEpoch() - segmentList.first()->first()->dateTime->toSecsSinceEpoch();

        twtmp = new QTreeWidgetItem(this);
        twtmp->setText(0,tr("Total time"));
        QString days;
        days.setNum(sec / (24*60*60));
        sec = sec % (24*60*60);
        QString hours;
        hours.setNum(sec/3600);
        sec = sec % 3600;
        QString minutes;
        minutes.setNum(sec/60);
        sec = sec % 60;
        QString seconds;
        seconds.setNum(sec);
        twtmp->setText(1,((days.toInt() > 0)?(days + tr(" Days ")):"") + hours + ":" + minutes + ":" + seconds);


        twtmp = new QTreeWidgetItem(this);
        twtmp->setText(0,tr("Route length"));
        double lenght = round(routeLenght) / 1000;
        QString rl = QString(tr("%1 km")).arg(lenght);
        twtmp->setText(1,rl);


        //////////////////////////////
        twtmp = new QTreeWidgetItem(this);
        int l = segmentList.length();
        if(l == 1) {
            twtmp->setText(0,QString(tr("%1 segment", "1")).arg(l));
        } else if(l >1 && l<5) {
            twtmp->setText(0,QString(tr("%1 segments", "2-4")).arg(l));
        } else {
            twtmp->setText(0,QString(tr("%1 segments", "jine")).arg(l));
        }

        for(int i=0; i<segmentList.length(); i++) {
            QTreeWidgetItem *tw2level = new QTreeWidgetItem(twtmp);
            tw2level->setText(0,QString("segment %1").arg(i));

            tw2level->setText(1,QString("%1 - %2 UTC")
                              .arg(segmentList.at(i)->first()->dateTime->toString(formatHandler->formatDateTime))
                              .arg(segmentList.at(i)->last()->dateTime->toString(formatHandler->formatDateTime)));

        }
        //////////////////////////////

        QBrush base("#F8FFFF");
        QBrush altBase("#EEFFFF");
        for(int i = 0; i<this->childCount(); i++) {
            this->child(i)->setBackground(0,i%2 ? base : altBase);
            this->child(i)->setBackground(1,i%2 ? base : altBase);
        }
    }
}

void GpsRoute::setGpsFormat(QAction *action) {

    qDebug() << "FIXME: ambigious conversion from int to QChar" << action->data().toInt();
    return; // FIXME
//    formatHandler->formatGps = action->data().toInt();
    double lat = segmentList.first()->first()->latitude;
    double lon = segmentList.first()->first()->longitude;
    this->child(3)->setText(1,formatHandler->gpsAllInFormat(lat,lon));

    lat = segmentList.last()->last()->latitude;
    lon = segmentList.last()->last()->longitude;
    this->child(4)->setText(1,formatHandler->gpsAllInFormat(lat,lon));
}

void GpsRoute::setDateTimeFormat(QAction *action) {
    formatHandler->formatDateTime = action->data().toString();
    this->child(1)->setText(1,segmentList.first()->first()->dateTime->toString(formatHandler->formatDateTime) +" UTC");
    this->child(2)->setText(1,segmentList.last()->last()->dateTime->toString(formatHandler->formatDateTime) +" UTC");
}

qint64 GpsRoute::startTime() {
    return segmentList.first()->first()->dateTime->toSecsSinceEpoch();
}

qint64 GpsRoute::endTime() {
    return segmentList.last()->last()->dateTime->toSecsSinceEpoch();
}

double GpsRoute::startLat() {
    return segmentList.first()->first()->latitude;
}

double GpsRoute::startLon() {
    return segmentList.first()->first()->longitude;
}

double GpsRoute::endLat() {
    return segmentList.last()->last()->latitude;
}

double GpsRoute::endLon() {
    return segmentList.last()->last()->longitude;
}
