/** @file gpspanel.cpp
  * Soubor s tridou GpsPanel dedici ze trify QDockWidget pro zobrazeni otevrenych GPS tras
  */

#include "gpspanel.h"

GpsPanel::GpsPanel( QWidget * parent):
        QDockWidget(parent)
{
    setWindowTitle(tr("GPS routes"));
    setVisible(false);
    setFeatures(QDockWidget::DockWidgetClosable |
                QDockWidget::DockWidgetMovable |
                QDockWidget::DockWidgetFloatable);

    setMinimumWidth(150);

    treeW = new QTreeWidget(this);
    treeW->setColumnCount(2);
    treeW->setColumnWidth(0,165);
    treeW->setColumnWidth(1,140);
    treeW->setHeaderLabels(QStringList() << "" << "");

    setWidget(treeW);
    resize(sizeHint());

}

void GpsPanel::retranslateUi()
{
    setWindowTitle(tr("GPS routes"));
}

QSize GpsPanel::sizeHint() const
{
    return QSize(280, 16777215);
}
void GpsPanel::addGpsWidget(GpsRoute *gpsRouteWidget)
{
    treeW->addTopLevelItem(gpsRouteWidget);
    treeW->setItemWidget(gpsRouteWidget, 1,  gpsRouteWidget->buttonWidget);
}
