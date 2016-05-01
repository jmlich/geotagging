/** @file imagewidgetslist.h
  * Hlavickovy soubor tridy ImageWidgetsList dedici ze trid QObject a QList<ImageInfo *>
  * obsahujici seznam widgetù s fotografiemi
  */

#ifndef IMAGEWIDGETSLIST_H
#define IMAGEWIDGETSLIST_H

#include <QObject>
#include <QList>
#include "imageinfo.h"
#include "mapwidget.h"
class ImageWidgetsList : public QObject, public QList<ImageInfo *>
{
    Q_OBJECT

public:
    explicit ImageWidgetsList(QObject *parent = 0);
    void addImage(ImageInfo *imageWidget);
    MapWidget *map;
    QList<int> selectedIdList();
    //int selectedCount;
    bool isNotSaved();

signals:
    void showImages();
    void setExifInfo(QStringList);
public slots:
    void deleteSelected();

    void saveExifGpsInSelected();
    void saveExifDateTimeInSelected();
    void saveExifGpsInAll();
    void saveExifDateTimeInAll();

    void testRightClick(QPoint p);
    void rubberBandSelection(QRect r, bool apply);

    void selectOne(int id, bool clickMarker, bool focus);

    //void changeLabelsVisibility(QAction*a);

};

#endif // IMAGEWIDGETSLIST_H
