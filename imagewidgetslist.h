/** @file imagewidgetslist.h
 * Hlavickovy soubor tridy ImageWidgetsList dedici ze trid QObject a QList<ImageInfo *>
 * obsahujici seznam widgetů s fotografiemi
 */

#ifndef IMAGEWIDGETSLIST_H
#define IMAGEWIDGETSLIST_H

#include "imageinfo.h"
#include "mapwidget.h"
#include <QList>
#include <QObject>
class ImageWidgetsList : public QObject, public QList<ImageInfo*> {
    Q_OBJECT

public:
    explicit ImageWidgetsList(QObject* parent = 0);
    void addImage(ImageInfo* imageWidget);
    MapWidget* map;
    QList<int> selectedIdList();
    int selectedIndex() const;
    void setGridColumnCount(int cols);
    // int selectedCount;
    bool isNotSaved();

private:
    int gridColumnCount = 1;

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
    int selectRelative(int indexOffset, int anchorIndex = -1);
    int selectRelativeRow(int rowOffset, int anchorIndex = -1);

    // void changeLabelsVisibility(QAction*a);
};

#endif // IMAGEWIDGETSLIST_H
