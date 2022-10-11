#include "aboutdialog.h"
#include <QFile>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QString verion_string = QString("%1 / %2 %3").arg(GIT_VERSION, __DATE__, __TIME__);
    ui.label_version->setText(verion_string);

    QString license_text = "";
    QFile file(":/LICENSE");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            license_text = license_text + line;
        }
    }

    ui.textBrowser->setText(license_text);
}
