/*************************************************************************
> FileName: host-address.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 11:05:31 AM CST
 ************************************************************************/
#include <QDebug>
#include <QDateTime>

int main (int argc, char* argv[])
{
    //auto src = QLocale::system();
    //QLocale::setDefault(QLocale(QLocale::English));
    auto tm = QDateTime::currentDateTime().toLocalTime();

    qDebug() << tm.toUTC();
    qDebug() << tm.toString("MMM d hh:mm:ss");
    qDebug() << tm.toString("MM d hh:mm:ss");
    //QLocale::setDefault(src);

    return 0;
}
