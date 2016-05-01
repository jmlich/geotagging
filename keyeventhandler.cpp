/** @file keyeventhandler.cpp
  * Soubor s tridou KeyEventHandler dedici ze tridy QObject,
  * slouží pro rozpoznani klavesovych zkratek
  */
#include "keyeventhandler.h"

KeyEventHandler::KeyEventHandler(QObject *parent) :
    QObject(parent)
{
}
void KeyEventHandler::processKeyEvent(QKeyEvent *event)
{
    if(event->key() == int('A') && (event->modifiers() & Qt::ControlModifier)){

        emit(selectImage());
    }
    else if(event->key() == Qt::Key_Delete){
        emit(deleteSelected());
    }
    else if(event->key() == Qt::Key_Escape){
        emit(escapePressed());
    }


}
