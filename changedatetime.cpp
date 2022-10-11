/** @file changedatetime.cpp
 * Soubor s tridou ChangeDateTime dedici ze tridy QWidget obsahujici box pro posun casu fotografie
 */

#include "changedatetime.h"
#include "ui_changedatetime.h"

ChangeDateTime::ChangeDateTime(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChangeDateTime)
{
    ui->setupUi(this);

    connect(ui->dateTimeEdit->timer, SIGNAL(timeout()), this, SLOT(showDateTime()));
    connect(ui->dateTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(processDateTimeChange(QDateTime)));
    connect(ui->radioButtonNewTime, SIGNAL(toggled(bool)), ui->groupBox, SLOT(setEnabled(bool)));
}
void ChangeDateTime::retranslateUi()
{
    ui->retranslateUi(this);
}

ChangeDateTime::~ChangeDateTime()
{
    delete ui;
}

void ChangeDateTime::showDateTime()
{
    ui->dateTimeEdit->timer->setInterval(1000);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(-timeShift()));
}

void ChangeDateTime::showEvent(QShowEvent*)
{
    ui->hourSpinBox->setValue(0);
    ui->minuteSpinBox->setValue(0);
    ui->secondSpinBox->setValue(0);
}

void ChangeDateTime::processDateTimeChange(QDateTime)
{
    int secChange = -(ui->dateTimeEdit->dateTime().toSecsSinceEpoch()
        - QDateTime::currentDateTime().toSecsSinceEpoch());
    if (-1 < secChange && secChange <= 1) {
        return;
    }
    ui->hourSpinBox->setValue(secChange / 3600);
    ui->minuteSpinBox->setValue((secChange % 3600) / 60);
    ui->secondSpinBox->setValue(secChange % 60);

    if (ui->secondSpinBox->value() >= 60 || ui->secondSpinBox->value() <= -60) {
        ui->minuteSpinBox->setValue(ui->minuteSpinBox->value() + ui->secondSpinBox->value() / 60);
        ui->secondSpinBox->setValue(ui->secondSpinBox->value() % 60);
    }
    if (ui->minuteSpinBox->value() >= 60 || ui->minuteSpinBox->value() <= -60) {
        ui->hourSpinBox->setValue(ui->hourSpinBox->value() + ui->minuteSpinBox->value() / 60);
        ui->minuteSpinBox->setValue(ui->minuteSpinBox->value() % 60);
    }
}

int ChangeDateTime::timeShift()
{
    long long int sec = ui->hourSpinBox->text().toInt() * 3600
        + ui->minuteSpinBox->text().toInt() * 60
        + ui->secondSpinBox->text().toInt();
    return sec;
}

int ChangeDateTime::isSaveTime()
{
    return ui->checkBox_isSaveTime->isChecked();
}

bool ChangeDateTime::isSetChange()
{
    return ui->radioButtonNewTime->isChecked();
}
