#include <iostream>
#include <QDebug>
#include "CareRayServerWidget.h"
#include <QApplication>
#include <QMutex>
#include <QString>
#include <QFile>
#include <QDir>
QMutex mutex1;
QString timepoint1;


//日志生成
void LogMsgOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    mutex1.lock();
    std::cout << msg.toStdString() << std::endl;
    //Critical Resource of Code
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray localMsg1 = msg.toUtf8();
    QString log;

    switch (type) {
    case QtDebugMsg:
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("[%5]Debug  File:%1 %2  Line:%3  Content:%4").arg(context.file).arg(context.function).arg(context.line).arg(msg).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss.zzz")));
        break;
    case QtInfoMsg:

        log.append(QString("[%5]Info  File:%1 %2  Line:%3  Content:%4").arg(context.file).arg(context.function).arg(context.line).arg(msg).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss.zzz")));
        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        //log.append(QString("[%5]Info: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss.zzz")));
        break;
    case QtWarningMsg:
        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("[%5]Warning: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss.zzz")));
        break;
    case QtCriticalMsg:
        //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("[%5]Critical: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss.zzz")));
        break;
    case QtFatalMsg:
        //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        log.append(QString("[%5]Fatal: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss.zzz")));
        abort();
    }
    QString path1 = QDir::currentPath();


    QFile file;
    QString path = QString("%1%2%3%4.lgt").arg(path1).arg("/log/").arg("log").arg(timepoint1);
    file.setFileName(path);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        QString erinfo = file.errorString();
        std::cout << erinfo.toStdString() << std::endl;
        return;
    }
    QTextStream out(&file);
    out << "\n\r" << log;
    file.close();

    mutex1.unlock();
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    qInstallMessageHandler(LogMsgOutput);
    qDebug() << "123";
    CareRayServerWidget window;
    window.show();
    return app.exec();
}