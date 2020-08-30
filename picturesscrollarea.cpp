/** @file picturesscrollarea.cpp
  * Soubor s tridou PicturesScrollArea dedici ze tridy QScrollArea,
  * implementuje skrolovaci plochu do ktere bude umisten ram s fotkami
  */

#include "picturesscrollarea.h"

PicturesScrollArea::PicturesScrollArea(QWidget *parent):
        QScrollArea(parent)
{
    setWidgetResizable(true);
    setFocusPolicy(Qt::StrongFocus );
    this->setFrameShape(QFrame::Panel);
    this->setFrameShadow(QFrame::Sunken);
    this->setBackgroundRole(QPalette::Light);

setAutoFillBackground(true);

}

void PicturesScrollArea::resizeEvent( QResizeEvent *event ) {
    event->accept();
    emit showImages();
}

QSize PicturesScrollArea::sizeHint() const {
    return QSize(850, 16777215);
}


void PicturesScrollArea::keyPressEvent ( QKeyEvent * event ) {
    emit(processEvent(event));
}

void PicturesScrollArea::wheelEvent(QWheelEvent * event) {
    if(event->modifiers() & Qt::ControlModifier){
        event->accept();
        if(event->delta() < 0) {
            emit(changeSize(-1));
        } else {
            emit(changeSize(1));
        }
    } else {
        QScrollArea::wheelEvent(event);
    }

}


void PicturesScrollArea::setImageFocus(int dy, int widgetHeight) {
    if(dy + this->widget()->pos().y() < 0) { //widget je nad scrollAreou
        QScrollBar *scrollBar = verticalScrollBar();
        scrollBar->setValue(scrollBar->value() +  (dy + this->widget()->pos().y()));
    } else if(dy > verticalScrollBar()->value() + this->contentsRect().height()-widgetHeight) { //widget je pod scrollAreou
        QScrollBar *scrollBar = verticalScrollBar();
        scrollBar->setValue(scrollBar->value() +  (dy + this->widget()->pos().y()) - (this->contentsRect().height()-widgetHeight));
    }
}

void PicturesScrollArea::shiftScrollBar(int dy) {
    verticalScrollBar()->setValue(verticalScrollBar()->value() +  dy*3);
}

