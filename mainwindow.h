/** @file mainwindow.h
 * Hlavickovy soubor tridy MainWindow dedici ze tridy QMainWindow,
 * implementuje hlavni okno aplikace
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutdialog.h"
#include "changedatetime.h"
#include "changetime.h"
#include "datetimedialog.h"
#include "exifinfo.h"
#include "gpspanel.h"
#include "gpsroute.h"
#include "gpsrouteslist.h"
#include "imagedata.h"
#include "imageinfo.h"
#include "imageview.h"
#include "imagewidgetslist.h"
#include "importfileprocessor.h"
#include "mapwidget.h"
#include "picturesframe.h"
#include "synchdialog.h"
#include "synchresultdialog.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLibraryInfo>
#include <QList>
#include <QMainWindow>
#include <QProgressBar>
#include <QResizeEvent>
#include <QSettings>
#include <QThread>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    QTranslator* translator;
    QTranslator* qtTranslator;
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    // void resizeEvent( QResizeEvent *event );
    // GpsRoute *gpsRoute;
    int formatGpsLabel;
    // QSlider *sizeSlider;
    QTime t;

    void saveSetting();
    void loadSetting();

private:
    Ui::MainWindow* ui;
    // QList<ImageInfo *> imageWidgetsList;
    ImageWidgetsList* imageWidgetsList;
    // QList<GpsRoute *> gpsRoutesList;
    GpsRoutesList* gpsRoutesList;

    // QGridLayout * layout;
    QGridLayout* layoutGps;
    // void showImages();
    // void addImageWidget(int row, int column, ImageInfo *imageW);
    void setMyLayout();
    void setGpsLayout();
    // int setRoute(QString gpsfileName);
    // int addImage(QString fileName);

    void setGpsInImages();
    void synchronizeImageGpsDialog();

    void synchAsk();
    // bool openGpsRoutes(QStringList &gpsfileNames);
    // int importSubdirs;
    // QStringList unrecognized;
    // void processDropFile(QString fileName, int recursion);
    // void showFinishDialog(QTreeWidget *tw);
    void createDateTimeDialog();
    QActionGroup* groupGpsFormat;
    QActionGroup* groupDateTimeFormat;

    QAction* localDateTimeAction;
    QAction* localShortDateTimeAction;
    QAction* dateTimeAction2;
    // void setGpsPanel();
    GpsPanel* gpsPanel;
    ExifInfo* exifInfo;
    MapWidget* map;

    QDockWidget* mapDockWidget;
    QDockWidget* gpsPanelDockWidget;
    QDockWidget* exifInfoDockPanel;

    int sizeWidgetMin;
    int sizeWidgetMax;
    AboutDialog* aboutDialog;
    SynchDialog* synchDialog;
    SynchResultDialog* synchResultDialog;
    PicturesFrame* picturesFrame;
    // QList<GpsRoute *> *sortJoinGpsRoutes();
    double distance(double lat1, double lon1, double lat2, double lon2);
    int idRouteCount;
    int idImageCount;
    KeyEventHandler* keyEH;
    DateTimeDialog* dateTimeDialog;
    // QDialog *finishDialog;

    ChangeDateTime* dateTimeChange;
    bool isNewMarkerSetting;
    void changeImagesDateTime(bool all);

    QString importFilesStr;
    QString synchStr;

    ImportFileProcessor* importFileProcessor;
    QThread* importThread;

    QProgressBar* progressBar;

signals:
    // void changeEventSignal(QEvent *e);
    void retranslateUiSignal();
    void processDropUrls(QList<QUrl>* urlList);
    void importImages(QStringList fileNames);
    void importGpsFiles(QStringList fileNames);
    void setGpsThread(QThread* newThread);
private slots:
    // void on_actionSave_Gps_triggered();
    // void on_actionSave_Datetime_triggered();
    void on_actionSynchronize_with_GPS_route_triggered();
    void synchronizeSelected();
    // void on_actionNum_triggered();
    // void on_actionGrade_triggered();
    void on_actionOpen_triggered();

    void on_actionNastavit_zpo_d_n_triggered();
    // void on_changeButton_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);
    void closeEvent(QCloseEvent* event);
public slots:

    //    void changeDateTime_clicked(int, int, int, int, bool);
    void changeDateTime_clicked();
    // void showImages();
    // void resizeImages(int size);
    // void imageClicked(int id);
    // void changeSize(int s);
    void openGpsRoutesFromDialog();
    void synchronizeImageGps();
    // void selectAll();
    //  void deleteSelected();
    void saveSynchChanges();
    void removeCameraMarker();
    void addNewMarker();
    void addNewMarkerFinished();
    void addNewObjectMarker();
    void removeObjectMarker();
    void addNewObjectMarkerFinished();
    void setCameraDirection();
    void mousePressEvent(QMouseEvent* event);
    // void switchDateTimeFormat(QAction *action);
    void deleteRoute(int id);
    void setRoute(GpsRoute* gpsRoute);
    void addImage(ImageData* imageData);
    void importFinished(QStringList*);
    void openUnrecognizedDialog(QStringList* unrecognizedList);
    void setProgressRange(int maxValue);
    // void updateProgress(int value);
    // void keyPressEvent(QKeyEvent *);
    void quitLoading();
};

#endif // MAINWINDOW_H
