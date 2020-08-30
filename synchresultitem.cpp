/** @file synchresultitem.cpp
  * Soubor s tridou SynchResultItem dedici ze trid QObject a QTreeWidgetItem,
  * radky s vysledky v dialogu s vysledky synchronizace
  */

#include "synchresultitem.h"

SynchResultItem::SynchResultItem(ImageInfo *image,QString cOk, QString cFailed, QString cOkUnchecked, QString cFailedChecked, QTreeWidget *parent) :
        QTreeWidgetItem(parent)
{
    colorFailed = cFailed;
    colorOk = cOk;
    colorOkUnchecked = cOkUnchecked;
    colorFailedChecked = cFailedChecked;

    colCount = parent->columnCount();
    checkBox = new QCheckBox();
    checkBox->setChecked(0);
    checkBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    checkBox->setMaximumWidth(20);
    QObject::connect(checkBox, SIGNAL(clicked()), this, SLOT(setItemColor()));

    QLabel *l = new QLabel(QFileInfo(image->imageData->pictureName).fileName());
    QWidget *w = new QWidget;
    w->setLayout(new QHBoxLayout);
    w->layout()->setSpacing(0);
    w->layout()->setMargin(1);
    w->layout()->setAlignment(Qt::AlignLeft);
    w->layout()->addWidget(checkBox);
    w->layout()->addWidget(l);
    parent->setItemWidget(this, 0, w);


    if(image->imageData->dateTime->isNull()
            || image->imageData->dateTime->toTime_t() == QDateTime::fromString(QString("0000:00:00 00:00:00"), "yyyy:MM:dd hh:mm:ss").toTime_t()) {
        //neni datum ve fotce
        this->setText(1,"-");
        this->setText(2,"-");
        this->setText(3,"-");
        this->setText(4,"-");
        this->setText(5,"-");
        for(int j = 0; j<parent->columnCount(); j++)
            this->setBackgroundColor(j,colorFailed);
        synchOk = 0;
        timeDiff = 9999999;
        checkBox->setEnabled(false);

    } else {
        this->setText(1,image->imageData->dateTime->toString("dd.M.yyyy hh:mm:ss"));
        if(image->approxMethod == 0) {   //nejblizsi bod

            this->setText(3,tr("nearest point (") + image->candidatePointTime->toString("dd.MM.yyyy hh:mm:ss") + ")");
            timeDiff = image->imageData->dateTime->toTime_t()- image->candidatePointTime->toTime_t();
            char sign = '+';
            if (timeDiff < 0) {
                timeDiff *= -1;
                sign = '-';
            }

            int days = timeDiff / (24 * 3600);
            QString dayName;
            if(days == 1) {
                dayName = tr("day");
            } else if(days == 2 || days == 3 || days == 4) {
                dayName = tr("days", "low");
            } else {
                dayName = tr("days", "high");
            }

            int h = (timeDiff % 24) / 3600;
            int m = (timeDiff % 3600) / 60;
            int s = (timeDiff % 60);
            QString str;
            str = sign
                  + (days > 0 ? QString::number(days) + dayName + " " : "")
                  + (h ? QString("%1h").arg(h) : "")
                  + ((m || h) ? QString("%1m").arg(m) : "")
                  + QString("%1s").arg(s);
            this->setText(4,str);
        } else if(image->approxMethod == 1) {
            this->setText(3,tr("from 2 points"));
            timeDiff = 0;
        } else if(image->approxMethod == 2) { //z vice bodu

            this->setText(3,tr("from more points"));
            timeDiff = 0;
        }

        if(image->candidateIsCorrect) {
            checkBox->setChecked(1);
            for (int j = 0; j<parent->columnCount(); j++)
                this->setBackgroundColor(j,colorOk);
            synchOk = 1;
        } else {   //prirazeni selhalo
            for (int j = 0; j<parent->columnCount(); j++) {
                this->setBackgroundColor(j,colorFailed);
            }
            synchOk = 0;

        }
        this->setText(2,image->gpsCandidadesString());
        this->setText(5,image->routeName);
    }

    if(image->imageData->isGps)
    {
        QFont font = this->font(2);
        font.setBold(true);
        this->setFont(2,font);
    }
}

void SynchResultItem::setChecked(bool n) {
    if(checkBox->isEnabled()) {
        checkBox->setChecked(n);
        setItemColor();
    }
}
void SynchResultItem::setItemColor() {

    QString currentColor;
    if(synchOk && checkBox->isChecked()) {
        currentColor = colorOk;
    } else if(!synchOk && !checkBox->isChecked()) {
        currentColor = colorFailed;
    } else if(synchOk && !checkBox->isChecked()) {
        currentColor = colorOkUnchecked;
    } else if(!synchOk && checkBox->isChecked()) {
        currentColor = colorFailedChecked;
    }

    for(int j = 0; j<colCount; j++) {
        this->setBackgroundColor(j, currentColor);
    }

}
