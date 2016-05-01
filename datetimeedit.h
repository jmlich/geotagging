/** @file datetimeedit.h
  * Hlavickovy soubor tridy DateTimeEdit dedici ze tridy QDateTimeEdit implementujici box s casem fotoaparatu
  */


#ifndef DATETIMEEDIT_H
#define DATETIMEEDIT_H

#include <QDateTimeEdit>
#include <QTimer>
#include <QKeyEvent>
class DateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    explicit DateTimeEdit(QWidget *parent = 0);
    QTimer *timer;

signals:
   // void change();

public slots:
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent *event);
};

#endif // DATETIMEEDIT_H
