#include <QApplication>
#include <QtCore>
#include "mainwindow.h"


// turns on logging of context (file+line number) in c++
#define QT_MESSAGELOGCONTEXT

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    QString txt;

    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    now.setOffsetFromUtc(offset);

#if defined(Q_OS_LINUX)
    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).exists()) {
        QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    QFile outFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() +"editor.log");
#elif (defined (Q_OS_WIN) || defined (Q_OS_WIN32) || defined (Q_OS_WIN64))
    QFile outFile("editor.log");
#else
    QFile outfile("editor.log");
#endif
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream ts(&outFile);

    QTextStream std_out(stdout, QIODevice::WriteOnly);
    QTextStream std_err(stderr, QIODevice::WriteOnly);

    switch (type) {
    case QtDebugMsg:
        txt = QString("%1 [D] %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        std_out << txt << Qt::endl ;
        break;
    case QtWarningMsg:
        txt = QString("%1 [W]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        std_out << txt << Qt::endl;
        break;
    case QtCriticalMsg:
        txt = QString("%1 [C]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        std_err << txt << Qt::endl;
        break;
    case QtFatalMsg:
        txt = QString("%1 [F]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        std_err << txt << Qt::endl;
        abort();
    default:
        txt = QString("%1 [O]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        std_err << txt << Qt::endl;
        break;

    }
    ts << txt << Qt::endl;

    outFile.close();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageHandler);

    MainWindow w;
    w.show();

    return a.exec();
}
