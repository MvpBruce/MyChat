#include "widgets/mainwindow.h"

#include <QApplication>
#include <qfile.h>
#include <QSettings>
#include <QDir>

void initQSS(QApplication& app)
{
    QFile qss(":/styles/assets/styles/stylesheet.qss");
    if (qss.open(QFile::ReadOnly))
    {
        qDebug("Open qss sucess");
        QString style = QLatin1String(qss.readAll());
        app.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug("Open qss failed");
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initQSS(a);

    QString iniPath = QDir::toNativeSeparators(a.applicationDirPath()) + QDir::separator() + "config.ini";
    QSettings st(iniPath, QSettings::IniFormat);
    QString strHost = st.value("GateServer/host").toString();
    QString strPort = st.value("GateServer/port").toString();
    strGateServerURL = "http://" + strHost + ":" + strPort;
    MainWindow w;
    w.show();
    return a.exec();
}
