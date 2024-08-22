/*************************************************************************
> FileName: host-address.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 11:05:31 AM CST
 ************************************************************************/
#include <QDebug>
#include <QLocale>
#include <QSysInfo>
#include <QStorageInfo>

int main (int argc, char* argv[])
{
    QLocale locale;
    QStorageInfo root = QStorageInfo("/");

    qDebug() << root.fileSystemType() << "\n";
    qDebug() << root.device() << "\n";
    qDebug() << root.displayName() << "\n";
    qDebug() << root.bytesTotal() << "(" << locale.formattedDataSize(root.bytesTotal()) << ")" << "\n";
}
