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

int main (int argc, char* argv[])
{
    QSysInfo info;

    qDebug() << info.buildAbi() << "\n";
    qDebug() << info.buildCpuArchitecture() << "\n";
    qDebug() << info.currentCpuArchitecture() << "\n";
    qDebug() << info.kernelType() << "\n";
    qDebug() << info.kernelVersion() << "\n";
    qDebug() << info.machineHostName() << "\n";
    qDebug() << info.machineUniqueId() << "\n";
    qDebug() << info.prettyProductName() << "\n";
    qDebug() << info.productType() << "\n";
    qDebug() << info.productVersion() << "\n";

    return 0;
}
