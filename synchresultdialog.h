/** @file synchresultdialog.h
  * Hlavickovy soubor tridy SynchResultDialog dedici ze tridy QDialog, ktera
  * implementuje dialog s vysledky synchronizace
  */

#ifndef SYNCHRESULTDIALOG_H
#define SYNCHRESULTDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QScrollArea>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLabel>
#include <QAction>
#include <QPushButton>
#include <QGroupBox>
#include <QTimeEdit>
#include "imageinfo.h"
#include "synchresultitem.h"
class SynchResultDialog : public QDialog
{
    Q_OBJECT
    QTreeWidget *tw;

    QString colorOk;
    QString colorFailed;
    QString colorOkUnchecked;
    QString colorFailedChecked;


    QLabel *ltOk;
    QLabel *ltFailed;
    QLabel *ltOkUnchecked;
    QLabel *ltFailedChecked;
    QLabel *ltBoldShow;
    QLabel *ltBold;
    QList<SynchResultItem *> treeWidgetList;
   /* QList<QTreeWidgetItem *> widgetList;
    QList<QCheckBox *> checkBoxList;
    QList<bool> synchOkList;
    QList<int> timeDiffList;*/

    QAction *checkItemsAction;
    QAction *uncheckItemsAction;

   // void setItemColor(int i);
    QGroupBox *createLegend();
    QGroupBox *legend;
    QPushButton *legendButton;
    QPushButton *checkButton;
    QTimeEdit *timeEdit;
    QLabel *checkDistLabel;
    //QGroupBox *legendBox;
public:
    explicit SynchResultDialog(QWidget *parent = 0);
    QScrollArea *sa;
    QCheckBox *saveExifCheckbox;
   // void setTreeWidget(QTreeWidget *tw);
    void setNewDialog();
    void addNewItem(ImageInfo *image, int existingGpsMode);
    void showDialog();
    QSize sizeHint();


signals:

public slots:
    void itemDoubleClicked(QTreeWidgetItem*,int);
    void checkSelectedItems();
    void uncheckSelectedItems();
  //  void checkIsCheckbox();
    void legendButtonToggled(bool n);
    void checkButtonClicked();
    void retranslateUi();

};

#endif // SYNCHRESULTDIALOG_H
