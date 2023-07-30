#include "mainwindow.h"

#include <QApplication>
#include <qglobal.h>

#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qint64 sec = QDateTime::fromString("2022-08-18 16:06:18", "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch();

    printf ("%d\n", sec);

    return a.exec();
}
