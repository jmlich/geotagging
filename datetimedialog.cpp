/** @file datetimedialog.cpp
  * Soubor s tridou DateTimeDialog dedici ze tridy QDialog obsahujici dialog pro posun casu fotografie
  */


#include "datetimedialog.h"

DateTimeDialog::DateTimeDialog(QWidget *parent) :
        QDialog(parent)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");

    setPalette(pal);

    setWindowTitle(QString::fromLocal8Bit("Změna času fotografií"));
    setLayout(new QVBoxLayout);

    QGroupBox *gBox = new QGroupBox;
    gBox->setLayout(new QVBoxLayout);
    rAll = new QRadioButton(tr("All pictures"));
    rAll->setChecked(true);
    rSelected = new QRadioButton(tr("Selected pictures"));
    gBox->setContentsMargins(QMargins(0,0,0,0));
    gBox->layout()->addWidget(rAll);
    gBox->layout()->addWidget(rSelected);

    QLabel *l = new QLabel;
    l->setAutoFillBackground(true);
    l->setBackgroundRole(QPalette::Light);
    l->setMaximumHeight(1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox;//(QDialogButtonBox::Apply & QDialogButtonBox::Cancel);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    buttonBox->addButton(QDialogButtonBox::Ok); //Apply

    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));


    this->layout()->addWidget(gBox);
    this->layout()->addWidget(l);
    this->layout()->addWidget(buttonBox);
}
void DateTimeDialog::retranslateUi()
{
    rAll->setText(tr("All pictures"));
    rSelected->setText(tr("Selected pictures"));
}
