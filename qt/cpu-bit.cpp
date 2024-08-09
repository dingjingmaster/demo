/*************************************************************************
> FileName: host-address.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 11:05:31 AM CST
 ************************************************************************/
#include <QDebug>
#include <QSysInfo>

int main (int argc, char* argv[])
{
    int bits = QSysInfo::WordSize;

    qDebug() << bits;
}
