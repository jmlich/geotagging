/** @file picturesframe.h
  * Hlavickovy soubor tridy PicturesFrame dedici ze tridy QFrame,
  * implementuje ram pro pro umisteni widgetu s fotografiemi
  */
#ifndef PICTURESFRAME_H
#define PICTURESFRAME_H

#include <QFrame>
#include <QRubberBand>
#include <QMouseEvent>
#include <QScrollArea>
#include <QAction>
#include <QCursor>
#include <QBasicTimer>
#include "imageinfo.h"
#include "imagewidgetslist.h"
class PicturesFrame : public QFrame
{
    Q_OBJECT
    bool dragEventInProgress;
    int countColCount();
    QGridLayout * layout;

    int imageSize;
    int maxImageSize;
    int minImageSize;

    QBasicTimer timer;
    void timerEvent(QTimerEvent *event);
public:
    explicit PicturesFrame(QWidget *parent = 0);
    void mousePressEvent( QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void addImage(ImageInfo *imageWidget);

    ImageWidgetsList *imageWidgetsList;

    QPoint origin;
    QRubberBand *rubberBand;
    int i;

signals:
    void rubberBandRect(QRect r, bool apply);

    void shiftScrollBar(int dy);
    void mousePress(QMouseEvent*e);
    void rightClick(QPoint p);
public slots:
    void showImages();
    void changeImageSize(int s);

};

#endif // PICTURESFRAME_H
