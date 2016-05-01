#ifndef CHANGETIME_H
#define CHANGETIME_H
#include <QDebug>
#include <QDialog>
#include <QLineEdit>
#include <QDateTime>


namespace Ui {
    class ChangeTime;
}

class ChangeTime : public QDialog {
    Q_OBJECT

    void checkTime(QLineEdit *lineEdit);
public:
    ChangeTime(QWidget *parent = 0);
    ~ChangeTime();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChangeTime *ui;

private slots:
   /* void on_secondLineEdit_textEdited(QString );
    void on_minuteLineEdit_textEdited(QString );
    void on_hourLineEdit_textEdited(QString );
    void on_dayLineEdit_textEdited(QString );
    void on_monthLineEdit_textEdited(QString );
    void on_yearLineEdit_textEdited(QString );
    */
    void on_buttonBox_accepted();

signals:
    void changeDateTime(int , int , int , int, bool);
};

#endif // CHANGETIME_H
