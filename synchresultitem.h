/** @file synchresultitem.h
 * Hlavickovy soubor tridy SynchResultItem dedici ze trid QObject a QTreeWidgetItem,
 * radky s vysledky v dialogu s vysledky synchronizace
 */

#ifndef SYNCHRESULTITEM_H
#define SYNCHRESULTITEM_H

#include "imageinfo.h"
#include <QCheckBox>
#include <QLabel>
#include <QObject>
#include <QTreeWidgetItem>
class SynchResultItem : public QObject, public QTreeWidgetItem {
    Q_OBJECT
    QString colorOk;
    QString colorFailed;
    QString colorOkUnchecked;
    QString colorFailedChecked;

    int colCount;

public:
    explicit SynchResultItem(ImageInfo* image, QString cOk, QString cFailed, QString cOkUnchecked, QString cFailedChecked, QTreeWidget* parent = 0);
    QCheckBox* checkBox;
    bool synchOk;
    int timeDiff;
    void setChecked(bool n);
signals:

public slots:
    void setItemColor();
};

#endif // SYNCHRESULTITEM_H
