/** @file picturesscrollarea.h
 * Hlavickovy soubor tridy PicturesScrollArea dedici ze tridy QScrollArea,
 * implementuje skrolovaci plochu do ktere bude umisten ram s fotkami
 */

#ifndef PICTURESSCROLLAREA_H
#define PICTURESSCROLLAREA_H

#include "keyeventhandler.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>

class PicturesScrollArea : public QScrollArea {
    Q_OBJECT
    virtual QSize sizeHint() const;

public:
    PicturesScrollArea(QWidget* parent = 0);
    void resizeEvent(QResizeEvent* event);

    void setMw(QMainWindow* MW);
    QMainWindow* mainWindow;
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);

signals:
    void showImages();
    void changeSize(int s);
    void processEvent(QKeyEvent* event);
public slots:
    void setImageFocus(int dy, int widgetHeight);
    void shiftScrollBar(int dy);
};

#endif // PICTURESSCROLLAREA_H
