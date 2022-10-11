/** @file picturesframe.h
 * Soubor s tridou PicturesFrame dedici ze tridy QFrame,
 * implementuje ram pro pro umisteni widgetu s fotografiemi
 */
#include "picturesframe.h"

PicturesFrame::PicturesFrame(QWidget* parent)
    : QFrame(parent)
{

    rubberBand = NULL;
    dragEventInProgress = 0;

    layout = new QGridLayout;
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setLayout(layout);
    setAutoFillBackground(true);

    imageSize = 4;
    maxImageSize = 7;
    minImageSize = 0;
}

void PicturesFrame::changeImageSize(int s)
{

    if (imageSize + s < minImageSize || imageSize + s > maxImageSize) {
        return;
    }
    imageSize += s;
    showImages();
}

int PicturesFrame::countColCount()
{

    if (imageWidgetsList->length() <= 0) {
        return 0;
    }

    int colCount = (static_cast<QScrollArea*>(parent())->size().width()) / (imageWidgetsList->at(0)->actualWidth() + 10);
    if (!colCount) {
        colCount = 1;
    }

    if (colCount > imageWidgetsList->length()) {
        colCount = imageWidgetsList->length();
    }

    return colCount;
}

void PicturesFrame::addImage(ImageInfo* imageWidget)
{

    imageWidget->setIconSize(imageSize);
    imageWidget->setLabels();
    int numberLine = -1;
    int j = imageWidgetsList->length() - 1;
    int colCount = countColCount();
    numberLine = j / colCount;

    layout->addWidget(imageWidget, numberLine, j % colCount);
}
void PicturesFrame::showImages()
{

    int numberLine = -1;
    if (imageWidgetsList->length() < 1) {
        return;
    }
    if (imageSize != imageWidgetsList->at(0)->iconSize) {
        imageWidgetsList->at(0)->resizeWidget(imageSize);
    }
    int colCount = countColCount();

    for (int i = 0; i < imageWidgetsList->length(); i++) {
        if (i % colCount == 0) {
            numberLine++;
        }

        if (imageSize != imageWidgetsList->at(i)->iconSize) {
            imageWidgetsList->at(i)->resizeWidget(imageSize);
        }
        layout->addWidget(imageWidgetsList->at(i), numberLine, i % colCount);
    }
}

void PicturesFrame::timerEvent(QTimerEvent* event)
{

    event->accept();
    int y = mapToParent(mapFromGlobal(cursor().pos())).y();
    if (y < 0) { // jsem kurzorem nad ramcem
        emit shiftScrollBar(y);
    } else if (y > static_cast<QScrollArea*>(parent())->contentsRect().height()) {
        emit shiftScrollBar(y - static_cast<QScrollArea*>(parent())->contentsRect().height());
    }

    rubberBand->setGeometry(QRect(origin, mapFromGlobal(cursor().pos())).normalized());

    if (i % 5) {
        emit rubberBandRect(QRect(origin, mapFromGlobal(cursor().pos())), 0);
    }
    i++;
}

void PicturesFrame::mousePressEvent(QMouseEvent* event)
{

    emit mousePress(event);
    if (event->button() == Qt::LeftButton) {
        origin = event->pos();
        i = 0;
        if (!rubberBand) {
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        }

        rubberBand->setGeometry(QRect(origin, QSize(0, 0))); // tady to nekdy pada
        rubberBand->show();
        dragEventInProgress = 1;
        emit rubberBandRect(QRect(origin, event->pos()), 0);

        timer.start(60, this);
    } else if (event->button() == Qt::RightButton) {
        emit rightClick(event->pos());
    }
}

void PicturesFrame::mouseMoveEvent(QMouseEvent* event)
{

    if (dragEventInProgress) {
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());

        if (i % 5) {
            emit rubberBandRect(QRect(origin, event->pos()), 0);
        }
        i++;
    }
}

void PicturesFrame::mouseReleaseEvent(QMouseEvent* event)
{

    if (event->button() == Qt::LeftButton) {
        timer.stop();
        dragEventInProgress = 0;
        rubberBand->hide();
        QRect r(origin, event->pos());
        emit rubberBandRect(r, 1);
    }
}
