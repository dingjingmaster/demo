#include "volume-manager.h"

#include <QCoreApplication>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    VolumeManager* vm = VolumeManager::instance();


    return a.exec();
}
