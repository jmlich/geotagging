/** @file scrollarea.cpp
  * Soubor s tridou ScrollArea dedici ze tridy QScrollArea,
  * implementuje skrolovaci plochu do ktere bude umistena fotografie ve sve puvodni velikosti
  */


#include "scrollarea.h"

ScrollArea::ScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
        this->setContentsMargins(0,0,0,0);
        this->setWidgetResizable(true);
}

void ScrollArea::wheelEvent(QWheelEvent * event) {
    if(event->modifiers() & Qt::ControlModifier){
        event->accept();
        if(event->angleDelta().y() < 0) {
            emit changeSize(-1);
        } else {
            emit changeSize(1);
        }
    } else {
        QScrollArea::wheelEvent(event);
    }
}
