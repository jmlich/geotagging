/** @file imagedinfo.h
  * Hlavickovy soubor tridy ImageInfo dedici ze tridy QWidget zobrazujici fotografii a popisky
  */


#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QWidget>
#include <QLabel>
#include <QImageReader>
#include <QDateTime>
#include <QMouseEvent>
#include <QFileInfo>
#include "imageview.h"
#include "exifreaderwriter.h"
#include "formathandler.h"
#include "imagedata.h"
#include <QTreeWidget>
#include <QCheckBox>


namespace Ui {
    class ImageInfo;
}

class ImageInfo : public QWidget {
    Q_OBJECT

    QString exifTimeFormat;
    QLabel *markerLabel;
    QLabel *markerSelectedLabel;
    FormatHandler *formatHandler;
    QString defaultStyleSheet;
    QString selectedStyleSheet;

    QSize imageSizeHint();
    QSize sizeHint();
    // void mouseMoveEvent ( QMouseEvent * event );
    void mouseDoubleClickEvent( QMouseEvent * event );
   // void mousePressEvent( QMouseEvent * event);
    //QString convertDegree(double l);
    QString currentStyleSheet();
  //  int gpsSource;  //1 = z fotografie
                    //2 = prirazene z trasy
                    //3 = prirazene rucne



public:
    //ImageInfo(int idd, QWidget *parent = 0);
    ImageInfo(ImageData *newImageData, QWidget *parent = 0);
    ~ImageInfo();
    ImageData *imageData;

    QAction *saveGpsAction;
    QAction *saveDateTimeAction;
    QAction *newCameraMarkerAction;
    QAction *newObjectMarkerAction;
    QAction *removeCameraMarkerAction;
    QAction *removeObjectMarkerAction;
    QAction *setDirectionAction;
    QAction *synchAction;
    QAction *openExternaly;

   /* QString pictureName;
    QDateTime *dateTime;
    QDateTime originalDateTime;
    QDateTime lastDateTimeSaved;
 //   bool isDateTimeChanged;*/

    void setLabels();
    int iconSize;
    QStringList exifInformation();


  //  QString gpsInFormat(double lat, double lon);
   // int formatGps;

    bool isClicked;
    bool isClickedOrig;

    double latitudeCandidate;
    double longitudeCandidate;
    double altitudeCandidate;
    bool candidateIsCorrect;
    int approxMethod;
    QDateTime *candidatePointTime;
    QString routeName;
    QCheckBox *synchCheckBox;
    //QTreeWidget *tw;

    void setGpsLabel();
    void setTimeLabel();
    void setAltitudeLabel();
    void setDirectionLabel();
    void setObjectPositionLabel();

    QString gpsString();
    QString gpsCandidadesString();
    //bool loadFile(QString pictureFName);
    void resizeWidget(int size);
    void setIconSize(int iconS);
    void setOrigDateTime( bool isSaveTime);

    void changeDateTime(int timeShift, bool isSaveTime);


    int actualWidth();
    void saveDateTime();
    void saveGps();
    //void saveExifTime();
   // void saveExifLatLon();
    void saveNewData(bool saveExif);


private:
    Ui::ImageInfo *ui;

public slots:
    void scaleFinished(bool);
    //void testRect(QRect r, bool isCtrl, bool apply);
    void checkMarkerClickedId(int iid);
    void rescaleFinished(QPixmap *p);
   // void setGps(double lat, double lon);
    void setAltitude(double alt);
    void setGpsFromMap(int id, double lat, double lon, double alt);
    void setObjectGpsFromMap(int id, double lat, double lon, double alt);
    void setCameraDirectionFromMap(int id, double direction, double angleOfView);
    void setGpsCandidates(double lat, double lon, double altitude, QDateTime pointTime, int method);
    void setCandidateIsCorrect(bool isCorrect);
    void setCandidateRouteName(QString routeName);
    void setTextLabels();
    //void setDegreesFormat();
    //void setDegreesMinFormat();
    void setGpsFormat(QAction*action);
    void setDateTimeFormat(QAction*action);
//    void setDateTimeFormat();

    //void setDateTime(QString exifTimeFormat , QDateTime dateTime);
    void click(bool focus);
    void unclick();
    void unselect();
    void select();
   // void saveExifGpsIfSelected();
   // void saveExifDateTimeIfSelected();
    void retranslateUi();
  //  void contextMenuEvent(QContextMenuEvent *event);

    void changeLabelVisibility(QAction *a);
    void openExternalEditor();

    void imageChanged();


signals:
    void clicked(int id);
    void mapAddCameraMarker(int id, double lat, double lon, double direction, double angleOfView);
    void mapAddObjectMarker(int id, double lat, double lon);
    void selected(int id, bool s);
    void getFocus(int dy, int widgetHeight);
    void deleteMarker(int id);
    void readExif(QString n);
    void saveExifTime(QString,QDateTime*);
    void saveExifGps(QString, double, double, double, double, double);
};

#endif // IMAGEINFO_H
