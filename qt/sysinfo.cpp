/*************************************************************************
> FileName: sysinfo.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 22 Aug 2024 04:06:19 PM CST
 ************************************************************************/
#include <QDebug>
#include <QLocale>
#include <QSysInfo>
#include <QStorageInfo>
#include <QStandardPaths>

int main (int argc, char* argv[])
{
    QSysInfo info;

    qDebug() << info.buildAbi();
    qDebug() << info.buildCpuArchitecture();
    qDebug() << info.currentCpuArchitecture();
    qDebug() << info.kernelType();
    qDebug() << info.kernelVersion();
    qDebug() << info.machineHostName();
    qDebug() << info.machineUniqueId();
    qDebug() << info.prettyProductName();
    qDebug() << info.productType();
    qDebug() << info.productVersion();

    qDebug() << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    return 0;
}
