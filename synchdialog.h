/** @file synchdialog.h
  * Hlavickovy soubor tridy SynchDialog dedici ze tridy QDialog,
  * implementuje dialog s nastavenim parametru synchronizace
  */


#ifndef SYNCHDIALOG_H
#define SYNCHDIALOG_H

#include <QDialog>
#include <QList>
#include "gpsroute.h"
#include "changedatetime.h"
#include <ctime>
namespace Ui {
    class SynchDialog;
}

class SynchDialog : public QDialog
{
    Q_OBJECT
    QStringList zonesList;
public:
    explicit SynchDialog(QList<GpsRoute*> *gpsRoutes, QWidget *mw, ChangeDateTime *dateTimeChangeWidget, QWidget *parent = 0);
    ~SynchDialog();
    void showGpsRoutes();
    void init();

QGridLayout * layoutGps;
    bool isTimeChange();
    //int timeChange();

    bool isCheckedGpsRoute(int i);
    uint maxRoutesDistM();
    uint maxRoutesDistTime();
    double offset();
    bool isSynchAll();
    QList<int> checkedRoutes();
    ChangeDateTime *changeDateTimeWidget;
    bool isAnyRouteChecked();
    int method();
    void setMethod(int m);
    int existingGpsMode();
    void setExistGpsMode(int m);
    void setSelectedOnly(bool value);
    bool isJoinSeg();

    void setJoinEnabled(bool value);
    void setMaxDistTime(int t);
    void setMaxDistM(int m);

private:
    Ui::SynchDialog *ui;
    QList<GpsRoute*> *gpsRoutes;
public slots:
    void showEvent(QShowEvent *);
    void retranslateUi();
};

#endif // SYNCHDIALOG_H
