#include <QCoreApplication>
#include <qglobal.h>
#include "sqlite-utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqliteUtils s;

    return a.exec();
}
