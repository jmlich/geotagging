/** @file exifinfo.h
  * Hlavickovy soubor tridy ExifInfo dedici ze trify QDockWidget pro zobrazeni seznamu metadat fotografie
  */

#ifndef EXIFINFO_H
#define EXIFINFO_H

#include <QDockWidget>
#include <QTreeWidget>
#include <QLayout>
class ExifInfo : public QDockWidget
{
    Q_OBJECT
    QTreeWidget *tw;
    QStringList labelNames();
    QList<QTreeWidgetItem *> itemsList;
public:
    explicit ExifInfo( QWidget * parent = 0);
    QSize sizeHint();
signals:

public slots:
    void retranslateUi();
    void setNewInfo(QStringList exifList);

};

#endif // EXIFINFO_H
