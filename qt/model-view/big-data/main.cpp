#include "main-window.h"

#include <QApplication>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
