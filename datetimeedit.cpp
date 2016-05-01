/** @file datetimeedit.h
  * Soubor s tridou DateTimeEdit dedici ze tridy QDateTimeEdit implementujici box s casem fotoaparatu
  */

#include "datetimeedit.h"

DateTimeEdit::DateTimeEdit(QWidget *parent) :
        QDateTimeEdit(parent)
{
    timer = new QTimer(this);
}
void DateTimeEdit::showEvent(QShowEvent *event)
{
    event->accept();
    setDateTime(QDateTime::currentDateTime());
    timer->start(1000);
}
void DateTimeEdit::hideEvent(QHideEvent *event)
{
    event->accept();
    timer->stop();
}

void DateTimeEdit::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    timer->setInterval(3000);
    QDateTimeEdit::keyPressEvent(event);
    //emit(change());
}
/*void DateTimeEdit::mousePressEvent(QMouseEvent *event)
{
    //timer->setInterval(1000);
    QDateTimeEdit::mousePressEvent(event);
    //emit(change());
}*/
