#include "changetime.h"
#include "ui_changetime.h"


ChangeTime::ChangeTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeTime)
{
    ui->setupUi(this);
    setWindowTitle(QApplication::translate("MainWindow", "Nastavit zpo\305\276d\304\233n\303\255", 0, QApplication::UnicodeUTF8));
    //setWindowTitle(QString::fromWCharArray(static_cast<wchar_t*>("Zmnit as")));

    connect(this, SIGNAL(changeDateTime(int,int,int,int,bool)),parent, SLOT(changeDateTime_clicked(int,int,int,int,bool)));
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    this->setPalette(pal);
}

ChangeTime::~ChangeTime()
{
    delete ui;
}

void ChangeTime::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ChangeTime::on_buttonBox_accepted()
{
    int year = ui->yearSpinBox->text().toInt();
    int month = ui->monthSpinBox->text().toInt();
    int day = ui->daySpinBox->text().toInt();
    int sec = ui->hourSpinBox->text().toInt() * 3600 + ui->minuteSpinBox->text().toInt() * 60 + ui->secondSpinBox->text().toInt();

    /*if(ui->signComboBox->currentText() == "-")
    {
       year = -year;
       month = -month;
       day = -day;
       sec = -sec;
    }*/

    /*QString n;
    if(ui->photoRButton->isChecked())
        n = "pictures";
    else
        n = "gps";*/
    emit changeDateTime(year, month, day, sec, ui->checkBox_isSaveTime->isChecked());
}
/*
void ChangeTime::checkTime(QLineEdit *lineEdit)
{
    QString text = lineEdit->text();
    if(!text.isEmpty() && !text.at(lineEdit->cursorPosition() - 1).isNumber())
    {
        text.remove((lineEdit->cursorPosition() - 1),1);
        lineEdit->setText(text);
    }
}

void ChangeTime::on_yearLineEdit_textEdited(QString )
{
    checkTime(ui->yearLineEdit);
}

void ChangeTime::on_monthLineEdit_textEdited(QString )
{
    checkTime(ui->monthLineEdit);
}

void ChangeTime::on_dayLineEdit_textEdited(QString )
{
    checkTime(ui->dayLineEdit);
}

void ChangeTime::on_hourLineEdit_textEdited(QString )
{
    checkTime(ui->hourLineEdit);
}

void ChangeTime::on_minuteLineEdit_textEdited(QString )
{
    checkTime(ui->minuteLineEdit);
}

void ChangeTime::on_secondLineEdit_textEdited(QString )
{
    checkTime(ui->secondLineEdit);
}
*/
