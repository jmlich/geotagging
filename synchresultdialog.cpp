/** @file synchresultdialog.cp
  * Soubor s tridou SynchResultDialog dedici ze tridy QDialog, ktera
  * implementuje dialog s vysledky synchronizace
  */

#include "synchresultdialog.h"

SynchResultDialog::SynchResultDialog(QWidget *parent) :
        QDialog(parent)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    setPalette(pal);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    colorFailed = "#FFAA88";//"#FFD8A7";
    colorOk = "#AFFFA6";
    colorOkUnchecked = "#FFFF77";
    colorFailedChecked = "#FFDD77";//"#FFE07F"; //"#FFD877";

    legend = createLegend();
    legendButton = new QPushButton("Legenda");

    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    buttonBox->addButton(QDialogButtonBox::Save);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox, SIGNAL(accepted()), parent, SLOT(saveSynchChanges()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    /////////////////////////////////////////////

    timeEdit = new QTimeEdit;
    timeEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    timeEdit->setDisplayFormat("hh:mm:ss");

    checkButton = new QPushButton(tr("Check"));
    checkButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(checkButton, SIGNAL(clicked()), this, SLOT(checkButtonClicked()));
    /////////////////////////////////////////////
    sa = new QScrollArea;
    saveExifCheckbox = new QCheckBox(tr("Save in pictures EXIF data"));
    QGridLayout *dialogLayout = new QGridLayout;

    checkDistLabel = new QLabel(tr("Check pictures with time distance <"));
    dialogLayout->addWidget(sa,0,0,1,4);
    dialogLayout->addWidget(checkDistLabel,1,0,1,1);
    dialogLayout->addWidget(timeEdit,1,1,1,1);
    dialogLayout->addWidget(checkButton,1,2,1,1);
    dialogLayout->addWidget(saveExifCheckbox,2,0,1,4);
    dialogLayout->addWidget(legendButton,3,0,1,4);
    dialogLayout->addWidget(legend,4,0,1,4);

    dialogLayout->addWidget(buttonBox,5,0,1,4);
    legend->setVisible(false);

    legendButton->setCheckable(true);
    legendButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(legendButton, SIGNAL(toggled(bool)), this,SLOT(legendButtonToggled(bool)) );

    this->setLayout(dialogLayout);

    sa->setBackgroundRole(QPalette::Light);
    sa->setWidgetResizable(true);

    tw = new QTreeWidget;
    sa->setWidget(tw);
    tw->setHeaderLabels(QStringList()
                        << tr("Picture")
                        << tr("Picture Time")
                        << tr("New coordinates")
                        << tr("Synchronisation type")
                        << tr("Time distance")
                        << tr("From route"));

    tw->setIndentation(0);
    connect(tw, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));

    tw->setSelectionMode(QAbstractItemView::ExtendedSelection);

    checkItemsAction = new QAction(tr("Check selected"),tw);
    connect(checkItemsAction, SIGNAL(triggered()), this, SLOT(checkSelectedItems()));
    uncheckItemsAction = new QAction(tr("Uncheck selected"),tw);
    connect(uncheckItemsAction, SIGNAL(triggered()), this, SLOT(uncheckSelectedItems()));


    tw->setContextMenuPolicy( Qt::ActionsContextMenu );
    tw->addAction(checkItemsAction);
    tw->addAction(uncheckItemsAction);


    this->adjustSize();
    this->resize(QSize(700,450));
    this->setWindowTitle(tr("Synchronisation"));


}

void SynchResultDialog::retranslateUi()
{
    this->setWindowTitle(tr("Synchronisation"));
    checkButton->setText(tr("Check"));
    checkItemsAction->setText(tr("Check selected"));
    uncheckItemsAction->setText(tr("Uncheck selected"));
    saveExifCheckbox->setText(tr("Save in pictures EXIF data"));
    tw->setHeaderLabels(QStringList()
                        << tr("Picture")
                        << tr("Picture Time")
                        << tr("New coordinates")
                        << tr("Synchronisation type")
                        << tr("Time distance")
                        << tr("From route"));
    checkDistLabel->setText(tr("Check pictures with time distance <"));
    legend->setTitle(tr("Legend"));

    ltOk->setText(tr("Correctly assigned coordinates, will be saved"));
    ltFailed->setText(tr("Picture time doesn't fit any route, no coordinatest will be saved"));
    ltOkUnchecked->setText(tr("Correctly assigned coordinates, won't be saved"));
    ltFailedChecked->setText(tr("Picture time doesn't fit any route, will be saved coordinates of nearest known point"));
    ltBold->setText(tr("Picture already has assigned coordinates"));
    ltBoldShow->setText(tr("Bold"));

}

void SynchResultDialog::legendButtonToggled(bool n)
{

    legend->setVisible(n);
    if(n)
        resize(width(), height() + legend->height() + layout()->contentsMargins().bottom());
    else
        resize(width(), height() - legend->height() - layout()->contentsMargins().bottom());
}

QGroupBox *SynchResultDialog::createLegend()
{
    QGroupBox *box = new QGroupBox(tr("Legend"));
    box->setAutoFillBackground(true);
    QPalette palBox = box->palette();
    palBox.setColor(QPalette::Window, "#FFFFFF");
    box->setPalette(palBox);

    QGridLayout *l = new QGridLayout;

    QLabel *lOk = new QLabel;
    QLabel *lOkU = new QLabel;
    QLabel *lFailed = new QLabel;
    QLabel *lFailedC = new QLabel;

    int size = lOk->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size, size);
    pixmap.fill(QColor(colorOk));
    lOk->setPixmap(pixmap);
    pixmap.fill(QColor(colorOkUnchecked));
    lOkU->setPixmap(pixmap);
    pixmap.fill(QColor(colorFailed));
    lFailed->setPixmap(pixmap);
    pixmap.fill(QColor(colorFailedChecked));
    lFailedC->setPixmap(pixmap);

    ltOk = new QLabel(tr("Correctly assigned coordinates, will be saved"));
    ltFailed = new QLabel(tr("Picture time doesn't fit any route, no coordinatest will be saved"));
    ltOkUnchecked  = new QLabel(tr("Correctly assigned coordinates, won't be saved"));
    ltFailedChecked = new QLabel(tr("Picture time doesn't fit any route, will be saved coordinates of nearest known point"));
    ltBoldShow = new QLabel(tr("Bold"));
    QFont font = ltBoldShow->font();
    font.setBold(true);
    ltBoldShow->setFont(font);
    ltBold = new QLabel(tr("Picture already has assigned coordinates"));

    l->addWidget(lOk, 0, 0);
    l->addWidget(ltOk, 0, 1);
    l->addWidget(lOkU, 1, 0);
    l->addWidget(ltOkUnchecked, 1, 1);
    l->addWidget(lFailed, 2, 0);
    l->addWidget(ltFailed,2,1 );
    l->addWidget(lFailedC, 3, 0);
    l->addWidget(ltFailedChecked,3,1);
    l->addWidget(ltBoldShow,4,0);
    l->addWidget(ltBold,4,1);

    l->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    box->setLayout(l);
    return box;

}

//pri vybrani "zaskrtnout vybrane" z kontextoveho menu
void SynchResultDialog::checkSelectedItems()
{
    QTreeWidgetItem *twi;
    foreach(twi, tw->selectedItems())
    {
        int i = tw->indexOfTopLevelItem(twi);
        treeWidgetList.at(i)->setChecked(true);
    }
}
void SynchResultDialog::uncheckSelectedItems()
{
    QTreeWidgetItem *twi;
    foreach(twi, tw->selectedItems())
    {
        int i = tw->indexOfTopLevelItem(twi);
        treeWidgetList.at(i)->setChecked(false);
    }
}



void SynchResultDialog::itemDoubleClicked(QTreeWidgetItem *twi, int col)
{
    int i = tw->indexOfTopLevelItem(twi);
    if(treeWidgetList.at(i)->checkBox->isEnabled()){
        treeWidgetList.at(i)->checkBox->toggle();
        treeWidgetList.at(i)->setItemColor();
    }
}

void SynchResultDialog::setNewDialog()
{
    tw->clear();
    treeWidgetList.clear();
}

void SynchResultDialog::addNewItem(ImageInfo *image, int existingGpsMode)
{
    SynchResultItem *widget = new SynchResultItem(image,colorOk, colorFailed, colorOkUnchecked, colorFailedChecked, tw);

    if(existingGpsMode == 1 && image->imageData->isGps)//tucne, nezaskrtnout
    {
        widget->checkBox->setChecked(false);
    }
    treeWidgetList.append(widget);

    image->synchCheckBox = widget->checkBox;
}

void SynchResultDialog::showDialog()
{
    for(int i = 0; i < tw->columnCount(); i++)
    {
        tw->resizeColumnToContents(i);
        tw->setColumnWidth(i,tw->columnWidth(i) + 5);
    }

    tw->adjustSize();
    sa->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sa->resize(tw->width(), (tw->height() > 500 ? 500 : tw->height()));

    resize(contentsRect().width() + 30, contentsRect().height() + 30);
    exec();

}

void SynchResultDialog::checkButtonClicked()
{
    int time = timeEdit->time().hour() * 3600 + timeEdit->time().minute() * 60 + timeEdit->time().second();
    for(int i = 0; i<treeWidgetList.length(); i++)
    {

        if(treeWidgetList.at(i)->timeDiff < time)
        {
            treeWidgetList.at(i)->setChecked(true);
        }
    }
}

QSize SynchResultDialog::sizeHint()
{
    return QSize(0,0);
}


