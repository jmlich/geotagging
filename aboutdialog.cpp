#include "aboutdialog.h"


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    ui.label_version->setText(GIT_VERSION);
}
