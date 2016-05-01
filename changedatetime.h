/** @file changedatetime.h
  * Hlavickovy soubor tridy ChangeDateTime dedici ze tridy QWidget obsahujici box pro posun casu fotografie
  */
#ifndef CHANGEDATETIME_H
#define CHANGEDATETIME_H

#include <QWidget>
#include <QDateTime>
#include "datetimeedit.h"
namespace Ui {
    class ChangeDateTime;
}

class ChangeDateTime : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeDateTime(QWidget *parent = 0);
    ~ChangeDateTime();
    int timeShift();
    int isSaveTime();

private:
    Ui::ChangeDateTime *ui;

public slots:
    void showDateTime();
    void processDateTimeChange(QDateTime);
    void showEvent(QShowEvent *);
    bool isSetChange();
    void retranslateUi();

signals:
};

#endif // CHANGEDATETIME_H
