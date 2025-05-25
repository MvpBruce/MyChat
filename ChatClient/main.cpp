#include "widgets/mainwindow.h"

#include <QApplication>
#include <qfile.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/styles/assets/styles/stylesheet.qss");
    if (qss.open(QFile::ReadOnly))
    {
        qDebug("Open qss sucess");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug("Open qss failed");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
