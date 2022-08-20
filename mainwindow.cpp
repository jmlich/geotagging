/** @file mainwindow.cpp
  * Soubor s tridou MainWindow dedici ze tridy QMainWindow,
  * implementuje hlavni okno aplikace
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMimeData>
#include <QActionGroup>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    translator = new QTranslator;

    if (translator->load(QLocale(), QLatin1String("com.github.jmlich.geotagging"), QLatin1String("_"), "./")) {
        bool installed = qApp->installTranslator(translator);
        qDebug() << "installTranslator()" << QLocale::system().name() << (installed ? "success" : "failed");
    } else if (translator->load(QLocale(), QLatin1String("com.github.jmlich.geotagging"), QLatin1String("_"), QLibraryInfo::path(QLibraryInfo::TranslationsPath) )) {
        bool installed = qApp->installTranslator(translator);
        qDebug() << "installTranslator()" << QLocale::system().name() << (installed ? "success" : "failed");
    } else {
        qDebug() << "translation.load() failed " << QLocale::system().name() << " falling back to English";

        if (translator->load(QLatin1String("com.github.jmlich.geotagging_en_US")   , "./")) {
            qApp->installTranslator(translator);
        } else if (translator->load(QLatin1String("com.github.jmlich.geotagging_en_US"), QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
            qApp->installTranslator(translator);
        }

    }

    progressBar = NULL;
    ui->setupUi(this);

    //zvetseni okna
    this->setWindowState(Qt::WindowMaximized);

    gpsRoutesList = new GpsRoutesList;
    imageWidgetsList = new ImageWidgetsList;
    picturesFrame = new PicturesFrame();
    picturesFrame = new PicturesFrame(ui->scrollAreaPictures);

    aboutDialog = new AboutDialog(this);
    dateTimeChange = new ChangeDateTime();
    synchDialog = new SynchDialog(gpsRoutesList, this,dateTimeChange, this );
    synchResultDialog = new SynchResultDialog(this);
    dateTimeDialog = new DateTimeDialog(this);
    keyEH = new KeyEventHandler;

    gpsPanel = new GpsPanel(this);
    map = new MapWidget(this);
    exifInfo = new ExifInfo(this);

    importThread = new QThread;
    importFileProcessor = new ImportFileProcessor;

    groupGpsFormat = new QActionGroup( this );
    groupDateTimeFormat = new QActionGroup( this );
    //groupImageDisplay = new QActionGroup( this );

    map->gpsRoutesList = gpsRoutesList;

    ui->actionSave_Gps->setIcon(QIcon(":/icons/saveGpsIcon.png"));
    ui->actionSave_Datetime->setIcon(QIcon(":/icons/saveTimeIcon.png"));
    ui->actionNastavit_zpo_d_n->setIcon(QIcon(":/icons/clock.png"));
    ui->actionSynchronize_with_GPS_route->setIcon(QIcon(":/icons/synch.png"));
    ui->actionOpen->setIconVisibleInMenu(true);
    ui->actionSave_Gps->setIconVisibleInMenu(true);
    ui->actionSave_Datetime->setIconVisibleInMenu(true);
    ui->actionNastavit_zpo_d_n->setIconVisibleInMenu(true);
    ui->actionSynchronize_with_GPS_route->setIconVisibleInMenu(true);


    setWindowTitle(tr("Geotagging"));
    setWindowIcon(QIcon(":/icons/geotagging.png"));

    //mapa
    connect(this, SIGNAL(retranslateUiSignal()), map, SLOT(retranslateUi()));
    addDockWidget(Qt::RightDockWidgetArea, map);
    ui->menuView->addAction(map->toggleViewAction());
    imageWidgetsList->map = map;

//    QPalette pal = this->palette();
//    pal.setColor(QPalette::Window, "#D0D0E7");

//    this->setPalette(pal);


    importFileProcessor->moveToThread(importThread);
    connect(this, SIGNAL(processDropUrls(QList<QUrl> *)),
            importFileProcessor,SLOT(processDropUrls(QList<QUrl> *)));
    connect(this, SIGNAL(importImages(QStringList)),
            importFileProcessor,SLOT(importImages(QStringList)));
    connect(this, SIGNAL(importGpsFiles(QStringList)),
            importFileProcessor,SLOT(importGpsFiles(QStringList)));
    connect(importFileProcessor, SIGNAL(importRouteFinished(GpsRoute*)),
            this, SLOT(setRoute(GpsRoute*)));
    connect(importFileProcessor, SIGNAL(importImageFinished(ImageData*)),
            this, SLOT(addImage(ImageData *)));
    connect(importFileProcessor, SIGNAL(finished(QStringList *)),
            this,SLOT(importFinished(QStringList*)));
    connect(importFileProcessor, SIGNAL(setProgressMaximum(int)),
            this,SLOT(setProgressRange(int)));
    connect(keyEH, SIGNAL(escapePressed()), this, SLOT(quitLoading()));
    idImageCount = 0;
    idRouteCount = 0;

    isNewMarkerSetting = 0;

    connect(picturesFrame, SIGNAL(rightClick(QPoint)), imageWidgetsList, SLOT(testRightClick(QPoint)));
    connect(this, SIGNAL(retranslateUiSignal()), dateTimeChange, SLOT(retranslateUi()));
    connect(this, SIGNAL(retranslateUiSignal()), synchDialog, SLOT(retranslateUi()));
    connect(this, SIGNAL(retranslateUiSignal()), synchResultDialog, SLOT(retranslateUi()));

    picturesFrame->imageWidgetsList = imageWidgetsList;
   // connect(this, SIGNAL(retranslateUiSignal()), picturesFrame, SLOT(retranslateUi()));
    connect(imageWidgetsList, SIGNAL(showImages()), picturesFrame, SLOT(showImages()));
    connect(picturesFrame, SIGNAL(rubberBandRect(QRect,bool)),
            imageWidgetsList, SLOT(rubberBandSelection(QRect,bool)));
    connect(imageWidgetsList, SIGNAL(setExifInfo(QStringList)), exifInfo, SLOT(setNewInfo(QStringList)));
    connect( map, SIGNAL(mClicked(int,bool,bool)), imageWidgetsList, SLOT(selectOne(int,bool,bool)));

    connect(ui->actionSave_Gps, SIGNAL(triggered()), imageWidgetsList, SLOT(saveExifGpsInAll()));
    connect(ui->actionSave_Datetime, SIGNAL(triggered()), imageWidgetsList, SLOT(saveExifDateTimeInAll()));
    connect(ui->action_About,SIGNAL(triggered()), aboutDialog, SLOT(show()));
    connect(ui->actionExit,SIGNAL(triggered()),this, SLOT(close()));

    ui->scrollAreaPictures->setWidget(picturesFrame);
    ui->scrollAreaPictures->setWidgetResizable(true);
    connect(ui->scrollAreaPictures, SIGNAL(showImages()), picturesFrame, SLOT(showImages()));


    connect(this, SIGNAL(retranslateUiSignal()), dateTimeDialog, SLOT(retranslateUi()));
    connect(dateTimeDialog, SIGNAL(accepted()), this, SLOT(changeDateTime_clicked()));

    connect(ui->scrollAreaPictures, SIGNAL(processEvent(QKeyEvent*)), keyEH, SLOT(processKeyEvent(QKeyEvent*)));
    connect(map, SIGNAL(processEvent(QKeyEvent*)), keyEH, SLOT(processKeyEvent(QKeyEvent*)));
    connect(keyEH, SIGNAL(deleteSelected()), imageWidgetsList, SLOT(deleteSelected()));

    connect(map, SIGNAL(settingNewMarkerFinished()), this, SLOT(addNewMarkerFinished()));

    connect(picturesFrame, SIGNAL(shiftScrollBar(int)), ui->scrollAreaPictures, SLOT(shiftScrollBar(int)));
    connect(ui->scrollAreaPictures, SIGNAL(changeSize(int)), picturesFrame, SLOT(changeImageSize(int)));
    connect(picturesFrame, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this, SIGNAL(retranslateUiSignal()), gpsPanel, SLOT(retranslateUi()));

    addDockWidget(Qt::RightDockWidgetArea, gpsPanel);
    ui->menuView->addAction(gpsPanel->toggleViewAction());

    connect(this, SIGNAL(retranslateUiSignal()), exifInfo, SLOT(retranslateUi()));
    addDockWidget(Qt::LeftDockWidgetArea, exifInfo);
    ui->menuView->addAction(exifInfo->toggleViewAction());



    //menu pro nastaveni formatu zobrazeni GPS souradnic

    ui->actionDegree->setActionGroup(groupGpsFormat);
    ui->actionDegree->setData("d");
    ui->actionDegreeMin->setActionGroup(groupGpsFormat);
    ui->actionDegreeMin->setData("dm");
    ui->actionDegreeMinSec->setActionGroup(groupGpsFormat);
    ui->actionDegreeMinSec->setData("dms");

    localDateTimeAction = new QAction(tr("Local setting") + " ("
                                               + QDateTime::currentDateTime().toString(QLocale::system().dateTimeFormat(QLocale::LongFormat))
                                               + ")" ,this);
    localDateTimeAction->setCheckable(true);
    localDateTimeAction->setData(QLocale::system().dateTimeFormat(QLocale::LongFormat));
    localDateTimeAction->setActionGroup(groupDateTimeFormat);
    ui->menuDateTimeFormat->addAction(localDateTimeAction);

    localShortDateTimeAction = new QAction(tr("Local setting - short") + " ("
                                                    + QDateTime::currentDateTime().toString(QLocale::system().dateTimeFormat(QLocale::ShortFormat))
                                                    + ")",this);
    localShortDateTimeAction->setCheckable(true);
    localShortDateTimeAction->setData(QLocale::system().dateTimeFormat(QLocale::ShortFormat));
    localShortDateTimeAction->setActionGroup(groupDateTimeFormat);
    ui->menuDateTimeFormat->addAction(localShortDateTimeAction);

    dateTimeAction2 = new QAction(tr("d.M.yyyy hh:mm:ss") + " ("
                                           + QDateTime::currentDateTime().toString("d.M.yyyy hh:mm:ss")
                                           + ")",this);
    dateTimeAction2->setCheckable(true);
    dateTimeAction2->setChecked(true);
    dateTimeAction2->setData("d.M.yyyy hh:mm:ss");
    dateTimeAction2->setActionGroup(groupDateTimeFormat);
    ui->menuDateTimeFormat->addAction(dateTimeAction2);

    ui->actionAltitude->setData("alt");
    ui->actionLatitude_and_Longitude->setData("latLon");
    ui->actionDate_and_Time->setData("dateTime");

    ui->mainToolBar->setVisible(0);

    //drag and drop
    setAcceptDrops(true);

    loadSetting();
}

void MainWindow::loadSetting() {
    QSettings settings("Geotagging", "VUT FIT");

    settings.beginGroup("menu");
    ui->actionDate_and_Time->setChecked(settings.value("timeVisible", 1).toBool());
    ui->actionLatitude_and_Longitude->setChecked(settings.value("latLonVisible", 1).toBool());
    ui->actionAltitude->setChecked(settings.value("altVisible", 1).toBool());

    QString gpsFormat = settings.value("gpsFormat", "dm").toString();
    QString timeFormat = settings.value("timeFormat", "d.M.yyyy hh:mm:ss").toString();

    foreach(QAction *a, groupGpsFormat->actions()) {
        if(a->data() == gpsFormat) {
            a->setChecked(true);
            break;
        }
    }

    foreach(QAction *a, groupDateTimeFormat->actions()) {
        if(a->data() == timeFormat) {
            a->setChecked(true);
            break;
        }
    }
    settings.endGroup();

    settings.beginGroup("synchDialog");

    synchDialog->setMethod(settings.value("gpsApprox", 2).toInt());
    synchDialog->setExistGpsMode(settings.value("existGpsMode", 0).toInt());
    synchDialog->setSelectedOnly(settings.value("selectedOnly", false).toBool());

    synchDialog->setJoinEnabled(settings.value("seg/isChecked", false).toBool());
    synchDialog->setMaxDistTime(settings.value("seg/maxTime", 3600).toInt());
    synchDialog->setMaxDistM(settings.value("seg/maxDist", 20).toInt());

    settings.endGroup();
}

void MainWindow::saveSetting()
{
    QSettings settings("Geotagging", "VUT FIT");

    settings.beginGroup("menu");
    settings.setValue("timeVisible", ui->actionDate_and_Time->isChecked());
    settings.setValue("latLonVisible", ui->actionLatitude_and_Longitude->isChecked());
    settings.setValue("altVisible", ui->actionAltitude->isChecked());
    foreach(QAction *a, groupGpsFormat->actions()) {
        if(a->isChecked()) {
            settings.setValue("gpsFormat", a->data());
            break;
        }
    }
    foreach(QAction *a, groupDateTimeFormat->actions()) {
        if(a->isChecked()) {
            settings.setValue("timeFormat", a->data());
            break;
        }
    }
    settings.endGroup();

    settings.beginGroup("synchDialog");

    settings.setValue("gpsApprox", synchDialog->method());
    settings.setValue("existGpsMode", synchDialog->existingGpsMode());
    settings.setValue("selectedOnly", !synchDialog->isSynchAll());

    settings.setValue("seg/isChecked", synchDialog->isJoinSeg());
    settings.setValue("seg/maxTime", synchDialog->maxRoutesDistTime());
    settings.setValue("seg/maxDist", synchDialog->maxRoutesDistM());

    settings.endGroup();

}

void MainWindow::setCameraDirection() {
    bool ok;

    double direction = QInputDialog::getDouble(
                this,
                tr("Set camera direction"),
                tr("Camera direction"),
                180, -361, 360, 1,
                &ok, Qt::WindowFlags(), 1);

    if (ok) {
        QList<int> idList = imageWidgetsList->selectedIdList();

        map->setttingCameraMarkerDirection(idList, (direction < -360) ? qQNaN() : direction);

        qDebug() << "setCameraDirection " << idList << " " << direction;
    }
}

void MainWindow::removeCameraMarker() {
    QList<int> idList = imageWidgetsList->selectedIdList();

    map->settingRemoveCameraMarker(idList);

    qDebug() << "removeCameraMarker" << idList;

}

void MainWindow::addNewMarker() {
    QCursor cursor;
    cursor.setShape(Qt::CrossCursor);

    QList<int> idList = imageWidgetsList->selectedIdList();

    this->setCursor(cursor);
    map->settingNewMarker(cursor, idList);
    isNewMarkerSetting = 1;

    qDebug() << "addNewMarker" << idList << cursor;

}

void MainWindow::addNewMarkerFinished() {
    QCursor cursor;
    cursor.setShape(Qt::ArrowCursor);

    this->setCursor(cursor);
    map->endSettingNewMarker(cursor);
    isNewMarkerSetting = 0;
}

void MainWindow::removeObjectMarker() {
    QList<int> idList = imageWidgetsList->selectedIdList();
    map->settingRemoveObjectMarker(idList);

    qDebug() << "removeCameraMarker" << idList;
}


void MainWindow::addNewObjectMarker() {
    QCursor cursor;
    cursor.setShape(Qt::CrossCursor);

    QList<int> idList = imageWidgetsList->selectedIdList();

    this->setCursor(cursor);
    map->settingNewObjectMarker(cursor, idList);
    isNewMarkerSetting = 1;

    qDebug() << "addNewObjectMarker" << idList << cursor;

}

void MainWindow::addNewObjectMarkerFinished() {
    QCursor cursor;
    cursor.setShape(Qt::ArrowCursor);

    this->setCursor(cursor);
    map->endSettingNewObjectMarker(cursor);
    isNewMarkerSetting = 0;
}

void MainWindow::importFinished(QStringList *unrecognizedList) {
    if(progressBar != NULL) {
        delete progressBar;
        progressBar = NULL;
    }

    if(unrecognizedList->length() > 0) {
        openUnrecognizedDialog(unrecognizedList);
    }
    delete unrecognizedList;

    if(synchDialog->isVisible()) {
        synchDialog->showGpsRoutes();
    } else if(gpsRoutesList->length() > 0 && imageWidgetsList->length() > 0) {
        synchAsk();
    }
}

void MainWindow::openUnrecognizedDialog(QStringList *unrecognizedList) {

    QDialog * dialog = new QDialog(this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QPalette pal = dialog->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    dialog->setPalette(pal);

    QScrollArea *sa = new QScrollArea;
    sa->setWidget(new QFrame);

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addWidget(new QLabel(tr("Couldn't recognize format of some files")));

    dialogLayout->addWidget(sa);
    dialogLayout->addWidget(buttonBox);
    dialogLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    dialog->setLayout(dialogLayout);

    QGridLayout *l = new QGridLayout;
    l->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    l->setContentsMargins(QMargins(5,5,5,5));
    l->setVerticalSpacing(3);

    foreach(QString file, *unrecognizedList) {
        l->addWidget(new QLabel(file));
    }
    sa->widget()->setLayout(l);
    sa->setWidgetResizable(true);
    sa->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    sa->setBackgroundRole(QPalette::Light);
    sa->setMaximumHeight(280);

    dialog->setWindowTitle(tr("Drag and drop"));
    dialog->exec();

}

void MainWindow::setProgressRange(int maxValue) {
    progressBar = new QProgressBar;

    ui->statusBar->addWidget(progressBar);
    progressBar->setRange(0,maxValue);
    progressBar->setValue(0);
    connect(importFileProcessor, SIGNAL(setProgressValue(int)), progressBar, SLOT(setValue(int)));
}


void MainWindow::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    event->acceptProposedAction();
    picturesFrame->setBackgroundRole(QPalette::Light);

    if(mimeData->hasUrls()) {

        importThread->start();

        QList<QUrl> *urlList = new QList<QUrl>;
        *urlList = mimeData->urls();

        emit processDropUrls(urlList);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    ui->scrollAreaPictures->setWidgetResizable(true);
    picturesFrame->setBackgroundRole(QPalette::Midlight);

    event->acceptProposedAction();

}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
    picturesFrame->setBackgroundRole(QPalette::Light);
}


MainWindow::~MainWindow() {
    saveSetting();
    /*  delete gpsRoutesList;
    delete imageWidgetsList;
    delete picturesFrame;
    delete map;
    delete dateTimeChange;
    delete synchDialog;
    delete synchResultDialog;
    delete dateTimeDialog;
    delete keyEH;
    delete gpsPanel;
    delete groupGpsFormat;
    delete groupDateTimeFormat;*/
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {

    if(imageWidgetsList->isNotSaved()) {
        QMessageBox *mb = new QMessageBox(QMessageBox::Question, tr("Save changes"),
                                          tr("Some changes in EXIF metadata weren't saved. Do you wish to save them now?"),
                                          QMessageBox::Cancel | QMessageBox::No  | QMessageBox::Yes);

        QPalette pal = mb->palette();
        pal.setColor(QPalette::Window, "#D0D0E7");
        mb->setPalette(pal);
        int ret = mb->exec();
        if (ret == QMessageBox::Yes) {
            imageWidgetsList->saveExifDateTimeInAll();
            imageWidgetsList->saveExifGpsInAll();
            event->accept();
        } else if(ret == QMessageBox::No) {
            event->accept();
        } else if(ret == QMessageBox::Cancel) {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

void MainWindow::setGpsInImages()
{
    bool all = synchDialog->isSynchAll();
    int existingGpsMode = synchDialog->existingGpsMode();
    if(!gpsRoutesList->isEmpty() && synchDialog->isAnyRouteChecked()) { //tady kontrola jestli bylo neco zaskrtnute


        synchResultDialog->setNewDialog();
        for(int i = 0; i < imageWidgetsList->count(); i++) {
            if(all || imageWidgetsList->at(i)->isClicked ) {
                if(existingGpsMode == 2 && imageWidgetsList->at(i)->imageData->isGps) { //nezobrazovat fotografie ktere uz maji souradnice
                    continue;
                }
                gpsRoutesList->setGpsInImage(imageWidgetsList->at(i), synchDialog->offset(),
                                             synchDialog->checkedRoutes(), synchDialog->maxRoutesDistM(),
                                             synchDialog->maxRoutesDistTime(), synchDialog->method(),
                                             synchDialog->isJoinSeg());
                synchResultDialog->addNewItem(imageWidgetsList->at(i), existingGpsMode);
                //////////////////////////////////
            }
        }
        synchResultDialog->showDialog();

    } else {
        QMessageBox *mb = new QMessageBox(QMessageBox::Information, tr("Synchronisation"),
                                          QString(tr("No route selected")),
                                          QMessageBox::Close);

        QPalette pal = mb->palette();
        pal.setColor(QPalette::Window, "#D0D0E7");
        mb->setPalette(pal);
        mb->exec();
    }
}

void MainWindow::saveSynchChanges() {

    bool all = synchDialog->isSynchAll();
    for(int i = 0; i< imageWidgetsList->length(); i++) {
        if (all || imageWidgetsList->at(i)->isClicked) {
            imageWidgetsList->at(i)->saveNewData(synchResultDialog->saveExifCheckbox->isChecked());
        }
    }
}


void MainWindow::setRoute(GpsRoute *gpsRoute) {
    connect(this, SIGNAL(setGpsThread(QThread *)), gpsRoute, SLOT(setThread(QThread*)));
    emit setGpsThread(this->thread());

    gpsRoute->createLabels();

    connect(this, SIGNAL(retranslateUiSignal()),gpsRoute, SLOT(retranslateUi()));
    connect(groupGpsFormat, SIGNAL(triggered(QAction*)),gpsRoute,SLOT(setGpsFormat(QAction*)));
    connect(groupDateTimeFormat, SIGNAL(triggered(QAction *)), gpsRoute, SLOT(setDateTimeFormat(QAction*)));
    connect(gpsRoute, SIGNAL(changeOpacity(int,int)), map, SLOT(changeRouteOpacity(int,int)));
    connect(gpsRoute, SIGNAL(lineWidthChanged(int,int)), map, SLOT(lineWidthChanged(int,int)));
    connect(gpsRoute, SIGNAL(changeRouteColor(int,QString)), map, SLOT(changeRouteColor(int,QString)));
    connect(gpsRoute, SIGNAL(deleteRoute(int)), map, SLOT(deleteRoute(int)));
    connect(gpsRoute, SIGNAL(deleteRoute(int)),this, SLOT(deleteRoute(int)));

    gpsRoutesList->addRoute(gpsRoute);

    gpsRoute->fillLabels();

    gpsPanel->addGpsWidget(gpsRoute);
    gpsPanel->setVisible(1);
    gpsRoute->setDateTimeFormat(groupDateTimeFormat->checkedAction());
    gpsRoute->setGpsFormat(groupGpsFormat->checkedAction());
    map->addRoute(gpsRoute);

}



void MainWindow::on_actionOpen_triggered() {
    QList<QUrl> fileNames = QFileDialog::getOpenFileUrls(this, tr("Open files"),
                                                          QDir::currentPath(),
                                                          tr("Images and routes (*.png *.jpg *.JPG *.jpeg *.JPEG *.tif *.raw *.rw2 *.mrw *.orf *.raf *.arw *.ari *.gpx);;Images (*.png *.jpg *.JPG *.jpeg *.JPEG *.tif *.raw *.rw2 *.mrw *.orf *.raf *.arw *.ari);;GPS files (*.gpx);; All files (*)"));

    if(fileNames.length() < 1) {
        return;
    }

    importFileProcessor->processDropUrls(&fileNames);

}


void MainWindow::deleteRoute(int id) {
    gpsRoutesList->deleteRoute(id);
}

void MainWindow::openGpsRoutesFromDialog() {
    QStringList gpsfileNames = QFileDialog::getOpenFileNames(this,tr("Open GPS File"),  QDir::currentPath(),
                                                             tr("GPS files (*.gpx);; All Files (*)"));;

    importThread->start();
    emit importGpsFiles(gpsfileNames);

}

void MainWindow::quitLoading() {
    importThread->terminate();
    if(progressBar != NULL) {
        delete progressBar;
        progressBar = NULL;
    }

    if(isNewMarkerSetting == 1) {
        addNewMarkerFinished();
    }
}

void MainWindow::addImage(ImageData *imageData) {
    ImageInfo *imageWidget = new ImageInfo(imageData);
    imageWidgetsList->addImage(imageWidget);
    connect(this, SIGNAL(retranslateUiSignal()),imageWidget, SLOT(retranslateUi()));
    /////////////////////////////
    connect( imageWidget, SIGNAL(getFocus(int, int)), ui->scrollAreaPictures, SLOT(setImageFocus(int, int)));

    connect(groupGpsFormat, SIGNAL(triggered(QAction*)),imageWidget,SLOT(setGpsFormat(QAction*)));
    connect(groupDateTimeFormat, SIGNAL(triggered(QAction *)), imageWidget, SLOT(setDateTimeFormat(QAction*)));
    connect(keyEH, SIGNAL(selectImage()), imageWidget, SLOT(select()));
    connect(imageWidget->newCameraMarkerAction, SIGNAL(triggered()), this, SLOT(addNewMarker()));
    connect(imageWidget->newObjectMarkerAction, SIGNAL(triggered()), this, SLOT(addNewObjectMarker()));
    connect(imageWidget->removeCameraMarkerAction, SIGNAL(triggered()), this, SLOT(removeCameraMarker()));
    connect(imageWidget->removeObjectMarkerAction, SIGNAL(triggered()), this, SLOT(removeObjectMarker()));
    connect(imageWidget->setDirectionAction, SIGNAL(triggered()), this, SLOT(setCameraDirection()));
    connect(imageWidget->synchAction, SIGNAL(triggered()), this, SLOT(synchronizeSelected()));
    connect(ui->menuDisplayedImageInformations, SIGNAL(triggered(QAction*)),
            imageWidget, SLOT(changeLabelVisibility(QAction*)));
    imageWidget->changeLabelVisibility(ui->actionDate_and_Time);
    imageWidget->changeLabelVisibility(ui->actionLatitude_and_Longitude);
    imageWidget->changeLabelVisibility(ui->actionAltitude);

    imageWidget->setDateTimeFormat(groupDateTimeFormat->checkedAction());
    imageWidget->setGpsFormat(groupGpsFormat->checkedAction());

    picturesFrame->addImage(imageWidget);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    event->accept();
    if (isNewMarkerSetting == 1) {
        addNewMarkerFinished();
    }
}


void MainWindow::synchAsk() {

    QMessageBox *askDialog = new QMessageBox(QMessageBox::Question, tr("File import"),
                                             tr("Do you wish to synchronize pictures with GPS route?"),
                                             QMessageBox::Yes | QMessageBox::No);

    QPalette pal = askDialog->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    askDialog->setPalette(pal);

    int ret = askDialog->exec();

    if(ret == QMessageBox::Yes) {
        synchronizeImageGpsDialog();
    }
}



void MainWindow::on_actionNastavit_zpo_d_n_triggered() {
    static_cast<QVBoxLayout *>(dateTimeDialog->layout())->insertWidget(2, dateTimeChange);
    //static_cast<QFrame *>(dateTimeDialog->layout()->itemAt(0)->widget())->layout()->addWidget(dateTimeChange);
    dateTimeDialog->exec();
}

void MainWindow::changeDateTime_clicked() {
    changeImagesDateTime(dateTimeDialog->rAll->isChecked());
}

void MainWindow::changeImagesDateTime(bool all) {
    ImageInfo *image;
    for(int i=0; i<imageWidgetsList->length(); i++) {
        image = imageWidgetsList->at(i);
        if (all || image->isClicked) {
            if (dateTimeChange->isSetChange()) {
                if (dateTimeChange->timeShift() != 0) {
                    image->changeDateTime(dateTimeChange->timeShift(), dateTimeChange->isSaveTime());
                }
            } else {
                image->setOrigDateTime(dateTimeChange->isSaveTime());
            }
        }
    }
}

void MainWindow::synchronizeSelected() {
    synchDialog->setSelectedOnly(true);
    synchronizeImageGpsDialog();
}

void MainWindow::on_actionSynchronize_with_GPS_route_triggered() {
    synchronizeImageGpsDialog();
}

void MainWindow::synchronizeImageGpsDialog() {
    synchDialog->init();
    synchDialog->exec();
}

void MainWindow::synchronizeImageGps() {
    if(synchDialog->isTimeChange()) { //provedu zmenu casu fotografii
        changeImagesDateTime(synchDialog->isSynchAll());
    }
    setGpsInImages();
}
