/** @file datetimedialog.h
  * Hlavickovy soubor tridy DateTimeDialog dedici ze tridy QDialog obsahujici dialog pro posun casu fotografie
  */

#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
class DateTimeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DateTimeDialog(QWidget *parent = 0);
    QRadioButton *rAll;
    QRadioButton *rSelected;

signals:

public slots:
    void retranslateUi();
};

#endif // DATETIMEDIALOG_H
