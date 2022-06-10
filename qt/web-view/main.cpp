#include "mainwindow.h"
#include <QtWebView>
#include <QApplication>

int main(int argc, char *argv[])
{
    QtWebView::initialize ();
    QGuiApplication a(argc, argv);
//    MainWindow w;
//    w.show();


    return a.exec();
}
