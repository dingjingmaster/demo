#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;

    QProgressIndicator *pIndicator = new QProgressIndicator(&w);
    pIndicator->setColor(Qt::blue);
    pIndicator->startAnimation();

    w.show();
    return a.exec();
}
