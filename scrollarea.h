/** @file scrollarea.h
  * Hlavickovy soubor tridy ScrollArea dedici ze tridy QScrollArea,
  * implementuje skrolovaci plochu do ktere bude umistena fotografie ve sve puvodni velikosti
  */

#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QScrollArea>
#include <QWheelEvent>
#include <QScrollBar>
class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *parent = 0);
    void wheelEvent(QWheelEvent * event);
signals:
    void changeSize(int);
public slots:

};

#endif // SCROLLAREA_H
