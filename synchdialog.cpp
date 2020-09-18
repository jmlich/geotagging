/** @file synchdialog.h
  * Soubor s tridou SynchDialog dedici ze tridy QDialog,
  * implementuje dialog s nastavenim parametru synchronizace
  */


#include "synchdialog.h"
#include "ui_synchdialog.h"

SynchDialog::SynchDialog(QList<GpsRoute*> *gpsR,QWidget *mw,ChangeDateTime *changeDateTime, QWidget *parent ) :
        QDialog(parent),
        ui(new Ui::SynchDialog)
{
    ui->setupUi(this);
    changeDateTimeWidget = changeDateTime;
    gpsRoutes = gpsR;

    connect(ui->openGpsPushButton, SIGNAL(pressed()), mw, SLOT(openGpsRoutesFromDialog()));
    connect(ui->buttonBox, SIGNAL(accepted()), mw, SLOT(synchronizeImageGps()));


    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, "#D0D0E7");
    this->setPalette(pal);

    pal = ui->groupBox_openGps->palette();
    pal.setColor(QPalette::Window, "#E1ECEC");

    ui->groupBox_openGps->setPalette(pal);
    ui->groupBox_changeTime->setPalette(pal);
    ui->groupBox_setPoint->setPalette(pal);
    ui->groupBox_timeZone->setPalette(pal);
    ui->groupBox_conflicts->setPalette(pal);
    ui->groupBox_spojeniTras->setPalette(pal);
    ui->groupBox_photo->setPalette(pal);

    setWindowTitle(tr("Synchronize pictures with GPS routes"));
    ui->scrollAreaGps->setBackgroundRole(QPalette::Light);
    layoutGps = new QGridLayout;
    layoutGps->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    layoutGps->setContentsMargins(QMargins(1,1,1,1));
    layoutGps->setVerticalSpacing(0);
    ui->frameGps->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->frameGps->setLayout(layoutGps);
    ui->scrollAreaGps->setAlignment(Qt::AlignTop|Qt::AlignLeft);


    showGpsRoutes();

    ui->comboBox_zones->addItem(tr("UTC-12:00 - Baker Island, Howland Island"), 12);
    ui->comboBox_zones->addItem(tr("UTC-11:00 - Samoa, American Samoa"), 11);
    ui->comboBox_zones->addItem(tr("UTC-10:00 - Hawaii, Papeete"), 10);
    ui->comboBox_zones->addItem(tr("UTC-9:00  - Marquesas Islands"), 9.5);
    ui->comboBox_zones->addItem(tr("UTC-9:00  - Anchorage, Fairbanks, Juneau"), 9);
    ui->comboBox_zones->addItem(tr("UTC-8:00  - Vancouver, Washington (state), Portland, Las Vegas, California, Baja California"), 8);
    ui->comboBox_zones->addItem(tr("UTC-7:00  - Alberta, Colorado, Arizona, Chihuahua, Sonora"), 7);
    ui->comboBox_zones->addItem(tr("UTC-6:00  - Chicago, Costa Rica, Dallas, El Salvador, Guatemala, Honduras, Houston, Manitoba, Mexico City, Nicaragua, Saskatchewan"), 6);
    ui->comboBox_zones->addItem(tr("UTC-5:00  - Ottawa, Toronto, Montreal, Boston, New York, North Carolina, Washington D.C., Georgia, Miami, Cuba, Jamaica, Haiti, Panama, Colombia, Continental Ecuador, Peru"), 5);
    ui->comboBox_zones->addItem(tr("UTC-4:30  - Venezuela"), 4.5);
    ui->comboBox_zones->addItem(tr("UTC-4:00  - Nova Scotia, Dominican Republic, Puerto Rico, Trinidad and Tobago, Amazonas, Bolivia, Continental Chile, Paraguay, San Luis Province"), 4);
    ui->comboBox_zones->addItem(tr("UTC-3:30  - Newfoundland"), 3.5);
    ui->comboBox_zones->addItem(tr("UTC-3:00  - Rio de Janeiro, S?o Paulo, Argentina (except San Luis Province), Uruguay, Nuuk"), 3);
    ui->comboBox_zones->addItem(tr("UTC-2:00  - Fernando de Noronha, South Georgia and the South Sandwich Islands"), 2);
    ui->comboBox_zones->addItem(tr("UTC-1:00  - Azores, Cape Verde"), 1);
    ui->comboBox_zones->addItem(tr("UTC-0:00  - Iceland, Faroe Islands, United Kingdom, Ireland, Continental Portugal, Madeira, Morocco, Senegal, Ghana, Côte d'Ivoire"), 0);
    ui->comboBox_zones->addItem(tr("UTC+1:00  - Albania, Slovenia, Macedonia, Norway, Sweden, Denmark, "
                                   "Germany, the Netherlands, Belgium, Metropolitan France, Switzerland, Austria, Poland, "
                                   "Czech Republic, Slovakia, Hungary, Continental Spain, Italy, Croatia, Serbia, "
                                   "Kosovo, Bosnia and Herzegovina, Tunisia, Algeria, Nigeria, Cameroon, Angola, Kinshasa"), -1);
    ui->comboBox_zones->addItem(tr("UTC+2:00  - Finland, Lithuania, Latvia, Estonia, Belarus, Ukraine, Romania, Bulgaria, "
                                   "Greece, Turkey, Cyprus, Syria, Lebanon, Jordan, Palestine, Israel, Egypt, Libya, Mozambique, "
                                   "Malawi, Zambia, Zimbabwe, South Africa"), -2);
    ui->comboBox_zones->addItem(tr("UTC+3:00  - Samara, Iraq, Saudi Arabia, Yemen, Sudan, Ethiopia, Somalia, Kenya, Uganda, Tanzania, Madagascar"), -3);
    ui->comboBox_zones->addItem(tr("UTC+3:30  - Iran"), -3.5);
    ui->comboBox_zones->addItem(tr("UTC+4:00  - Georgia, Armenia, Azerbaijan, United Arab Emirates, Oman, Seychelles, Mauritius, Moscow, Saint Petersburg"), -4);
    ui->comboBox_zones->addItem(tr("UTC+4:30  - Afghanistan"), -4.5);
    ui->comboBox_zones->addItem(tr("UTC+5:00  - Sverdlovsk, Uzbekistan, Pakistan, Maldives, Kazakhstan"), -5);
    ui->comboBox_zones->addItem(tr("UTC+5:30  - India, Sri Lanka"), -5.5);
    ui->comboBox_zones->addItem(tr("UTC+5:45  - Nepal"), -5.75);
    ui->comboBox_zones->addItem(tr("UTC+6:00  - Novosibirsk, Almaty, Bangladesh"), -6);
    ui->comboBox_zones->addItem(tr("UTC+6:30  - Myanmar, Cocos Islands"), -6.5);
    ui->comboBox_zones->addItem(tr("UTC+7:00  - Krasnoyarsk, Thailand, Vietnam, Jakarta"), -7);
    ui->comboBox_zones->addItem(tr("UTC+7:30  - Java time"), -7.5);
    ui->comboBox_zones->addItem(tr("UTC+8:00  - Irkutsk, Ulan Bator, China, Taiwan, Hong Kong, Philippines, Malaysia, Singapore, Western Australia"), -8);
    ui->comboBox_zones->addItem(tr("UTC+9:00  - Zabaykalsky, Japan, North Korea, South Korea, East Timor"), -9);
    ui->comboBox_zones->addItem(tr("UTC+9:30  - Northern Territory, South Australia"), -9.5);
    ui->comboBox_zones->addItem(tr("UTC+10:00 - Victoria, Tasmania, Queensland, New South Wales, Primorsky"), -10);
    ui->comboBox_zones->addItem(tr("UTC+10:30 - Lord Howe Island"), -10.5);
    ui->comboBox_zones->addItem(tr("UTC+11:00 - Kamchatka, Solomon Islands, New Caledonia"), -11);
    ui->comboBox_zones->addItem(tr("UTC+11:30 - Norfolk Island"), -11.5);
    ui->comboBox_zones->addItem(tr("UTC+12:00 - Fiji, New Zealand"), -12);
    ui->comboBox_zones->addItem(tr("UTC+12:45 - Chatham Islands"), -12.45);
    ui->comboBox_zones->addItem(tr("UTC+13:00 - Tonga"), -13);
    ui->comboBox_zones->addItem(tr("UTC+14:00 - Line Islands"), -14);

    tzset();
#ifdef _XOPEN_SOURCE
    for(int i=0;i<ui->comboBox_zones->count(); i++) {
        if(ui->comboBox_zones->itemData(i).toDouble()*3600 == timezone)
            //if(ui->comboBox_zones->itemData(i).toDouble() == timezoneOffset)
        {
            ui->comboBox_zones->setCurrentIndex(i);
        }
    }
    if(daylight) {
        ui->checkBoxDST->setChecked(true);
    }
#endif // _XOPEN_SOURCE
}

void SynchDialog::retranslateUi()
{
    ui->retranslateUi(this);
    setWindowTitle(tr("Synchronize pictures with GPS routes"));
}

SynchDialog::~SynchDialog()
{
    delete ui;
}
void SynchDialog::showEvent(QShowEvent *)
{
    ui->groupBox_changeTime->layout()->addWidget(changeDateTimeWidget);
}

void SynchDialog::showGpsRoutes()
{
    for(int i=0, count=layoutGps->count(); i<count;i++)
    {
        layoutGps->itemAt(0)->widget()->setVisible(0);
        layoutGps->removeItem(layoutGps->itemAt(0));
    }

    for(int i=0; i<gpsRoutes->length(); i++)
    {
        QCheckBox *gpsCB = new QCheckBox(gpsRoutes->at(i)->name);
        gpsCB->setChecked(1);
        gpsCB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        gpsCB->setFixedHeight(22);
        gpsCB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        layoutGps->addWidget(gpsCB,i,0);
    }
}
void SynchDialog::init() {
    showGpsRoutes();
}

bool SynchDialog::isTimeChange() {
    return ui->groupBox_changeTime->isChecked();
}

bool SynchDialog::isCheckedGpsRoute(int i)
{
    if(layoutGps->count() <= i)     {
        return 0;
    }
    return static_cast<QCheckBox *>(layoutGps->itemAt(i)->widget())->isChecked();
}

uint SynchDialog::maxRoutesDistM() {
    return ui->spinBoxMaxDistM->value();
}

uint SynchDialog::maxRoutesDistTime() {
    return ui->spinBoxMaxDistTime->value();
}

double SynchDialog::offset() {
    double n = ui->comboBox_zones->itemData(ui->comboBox_zones->currentIndex()).toDouble();
    if (ui->checkBoxDST->isChecked()) {
        n--;
    }
    return n ;
}
bool SynchDialog::isSynchAll()
{
    if(ui->radioButton_all->isChecked())
        return true;
    else
        return false;
}
QList<int>  SynchDialog::checkedRoutes() {
    QList<int>  list;
    for(int i = 0; i<layoutGps->count(); i++) {
        list << static_cast<QCheckBox *>(layoutGps->itemAt(i)->widget())->isChecked();
    }
    return list;
}
bool SynchDialog::isAnyRouteChecked() {
    for (int i = 0; i<layoutGps->count(); i++) {
        if (static_cast<QCheckBox *>(layoutGps->itemAt(i)->widget())->isChecked()) {
            return 1;
        }
    }
    return 0;
}

int SynchDialog::method() {
    if(ui->radioNearestPoint->isChecked()) {
        return 0;
    } else if(ui->radio2points->isChecked()) {
        return 1;
    } else {
        return 2;
    }

}
void SynchDialog::setMethod(int m)
{
    if(m == 0) {
        ui->radioNearestPoint->setChecked(true);
    } else if(m == 1) {
        ui->radio2points->setChecked(true);
    } else {
        ui->radioMorePoints->setChecked(true);
    }
}

int SynchDialog::existingGpsMode()
{
    if(ui->radioIgnoreOld->isChecked()) {
        return 0;
    } else if(ui->radiocoordDontCheck->isChecked()) {
        return 1;
    } else {
        return 2;
    }
}
void SynchDialog::setExistGpsMode(int m)
{
    if(m == 0)
        ui->radioIgnoreOld->setChecked(true);
    else if(m == 1)
        ui->radiocoordDontCheck->setChecked(true);
    else
        ui->radioDontShow->setChecked(true);
}
void SynchDialog::setSelectedOnly(bool value)
{
    if (value) {
        ui->radioButton_selected->setChecked(true);
    } else {
        ui->radioButton_all->setChecked(true);
    }
}

bool SynchDialog::isJoinSeg() {
    return ui->groupBox_spojeniTras->isChecked();
}

void SynchDialog::setJoinEnabled(bool value) {
    ui->groupBox_spojeniTras->setChecked(value);
}

void SynchDialog::setMaxDistTime(int t) {
    ui->spinBoxMaxDistTime->setValue(t);
}

void SynchDialog::setMaxDistM(int m) {
    ui->spinBoxMaxDistM->setValue(m);
}
