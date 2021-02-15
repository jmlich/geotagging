/** @file exifinfo.cpp
  * Soubor s tridou ExifInfo dedici ze trify QDockWidget pro zobrazeni seznamu metadat fotografie
  */

#include "exifinfo.h"

ExifInfo::ExifInfo( QWidget * parent) :
    QDockWidget(parent)
{

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setWindowTitle(tr("EXIF Info"));
    tw = new QTreeWidget;

    setVisible(false);

    tw->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    tw->setHeaderLabels(QStringList() << tr("parameter") << tr("value"));
    tw->setSelectionMode(QAbstractItemView::NoSelection);

    this->setWidget(tw);

    QBrush base("#F8FFFF");
    QBrush altBase("#EEFFFF");

    QStringList labels = labelNames();
    for(int i=0; i<labels.length(); i++) {
        QTreeWidgetItem *twtmp = new QTreeWidgetItem(tw);
        twtmp->setText(0,labels.at(i));
        twtmp->setBackground(0,i%2 ? base : altBase);
        twtmp->setBackground(1,i%2 ? base : altBase);
        tw->setIndentation(0);
        itemsList.append(twtmp);
    }

}
QStringList ExifInfo::labelNames() {
    QStringList labelsNames;
    labelsNames << tr("Picture name")
                << tr("Path to file")
                << tr("Date and time")
                << tr("Latitude")
                << tr("Longitude")
                << tr("Altitude")
                << tr("Direction")
                << tr("Camera make")
                << tr("Camera model")
                << tr("Exposure time")
                << tr("Exposure Bias")
                << tr("Exposure Program")
                << tr("Resolution")
                << tr("Flash")
                << tr("F-number (F-spot)")
                << tr("Metering mode")
                << tr("ISO speed")
                << tr("Focal length")
                << tr("Exif comment")
//                << tr("Field Of View")
                   ;
    return labelsNames;
}

void ExifInfo::retranslateUi() {
    tw->setHeaderLabels(QStringList() << tr("parameter") << tr("value"));
    QStringList labels = labelNames();
    for(int i=0; i<labels.length() && i<itemsList.length(); i++) {
        itemsList.at(i)->setText(0,labels.at(i));
    }
}

void ExifInfo::setNewInfo(QStringList exifList) {
    while(exifList.length() < itemsList.length()) {
        exifList.append("");
    }
    for(int i=0; i<exifList.length() && i<itemsList.length(); i++) {
        itemsList.at(i)->setText(1,exifList.at(i));
    }
}

QSize ExifInfo::sizeHint() {
    return QSize(900, 1000);
}
