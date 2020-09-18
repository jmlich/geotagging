#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    Ui::AboutDialog ui;

signals:

};

#endif // ABOUTDIALOG_H
