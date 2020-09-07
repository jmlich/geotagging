/** @file imageview.h
  * Hlavickovy soubor tridy ImageView dedici ze tridy QWidget
  * pro zobrazeni noveho okna s fotografii v puvodni velikosti
  */

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "exiv2/exiv2.hpp"
#include "scrollarea.h"
//#include <QMouseEvent>

class ImageView : public QWidget {
    Q_OBJECT

    QLabel *imageL;
    QImage *img;
    ScrollArea *scrollArea;
    void resizeEvent( QResizeEvent *event );
    QSize currentSize;

public:
    ImageView(QWidget *parent = 0);
    ~ImageView();
    //ImageView(QWidget *parent = 0,QImage *image = 0);
    void setImage(QString pictureName);
   // void wheelEvent(QWheelEvent * event);
    QSize sizeHint() const;

public slots:
    void changeImageSize(int delta);

};

#endif // IMAGEVIEW_H
