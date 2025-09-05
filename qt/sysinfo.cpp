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

    qInfo() << info.buildAbi();
    qInfo() << info.buildCpuArchitecture();
    qInfo() << info.currentCpuArchitecture();
    qInfo() << info.kernelType();
    qInfo() << info.kernelVersion();
    qInfo() << info.machineHostName();
    //qInfo() << info.machineUniqueId();
    qInfo() << info.prettyProductName();
    qInfo() << info.productType();
    qInfo() << info.productVersion();

    qInfo() << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    return 0;
}
