/** @file keyeventhandler.cpp
 * Soubor s tridou KeyEventHandler dedici ze tridy QObject,
 * slouzi pro rozpoznani klavesovych zkratek
 */
#include "keyeventhandler.h"

KeyEventHandler::KeyEventHandler(QObject* parent)
    : QObject(parent)
{
}

void KeyEventHandler::processKeyEvent(QKeyEvent* event)
{

    if (event->key() == int('A') && (event->modifiers() & Qt::ControlModifier)) {
        emit selectImage();
    } else if (event->key() == Qt::Key_Delete) {
        emit deleteSelected();
    } else if (event->key() == Qt::Key_Escape) {
        emit escapePressed();
    } else if (!(event->modifiers() & Qt::ControlModifier)) {
        switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Backspace:
            emit selectAdjacent(-1);
            event->accept();
            return;
        case Qt::Key_Right:
        case Qt::Key_Space:
            emit selectAdjacent(1);
            event->accept();
            return;
        case Qt::Key_Up:
            emit selectAdjacentRow(-1);
            event->accept();
            return;
        case Qt::Key_Down:
            emit selectAdjacentRow(1);
            event->accept();
            return;
        default:
            break;
        }
    }
}
