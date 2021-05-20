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
    QFile outFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() +"geotagging.log");
#elif (defined (Q_OS_WIN) || defined (Q_OS_WIN32) || defined (Q_OS_WIN64))
    QFile outFile("geotagging.log");
#else
    QFile outfile("geotagging.log");
#endif
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream ts(&outFile);

    QTextStream std_out(stdout, QIODevice::WriteOnly);
    QTextStream std_err(stderr, QIODevice::WriteOnly);

    switch (type) {
    case QtDebugMsg:
        txt = QString("%1 [D] %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
#if QT_VERSION < 0x051400
        std_out << txt << endl ;
#else
        std_out << txt << Qt::endl ;
#endif
        break;
    case QtWarningMsg:
        txt = QString("%1 [W]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
#if QT_VERSION < 0x051400
        std_out << txt << endl ;
#else
        std_out << txt << Qt::endl ;
#endif
        break;
    case QtCriticalMsg:
        txt = QString("%1 [C]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
#if QT_VERSION < 0x051400
        std_out << txt << endl ;
#else
        std_out << txt << Qt::endl ;
#endif
        break;
    case QtFatalMsg:
        txt = QString("%1 [F]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
#if QT_VERSION < 0x051400
        std_out << txt << endl ;
#else
        std_out << txt << Qt::endl ;
#endif
        abort();
    default:
        txt = QString("%1 [O]: %2:%3 @ %4(): %5").arg(now.toString(Qt::ISODate)).arg(context.file).arg(context.line).arg(context.function).arg(msg);
#if QT_VERSION < 0x051400
        std_out << txt << endl ;
#else
        std_out << txt << Qt::endl ;
#endif
        break;

    }
#if QT_VERSION < 0x051400
    ts << txt << endl;
#else
    ts << txt << Qt::endl;
#endif

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
