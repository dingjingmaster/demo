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
    auto tm = QDateTime::fromString("15:14:59", "h:m:s");

    qDebug() << "           UTC: " << tm.toUTC();
    qDebug() << "MMM d hh:mm:ss: " << tm.toString("MMM d hh:mm:ss");
    qDebug() << " MM d hh:mm:ss: " << tm.toString("MM d hh:mm:ss");
    qDebug() << "             y: " << tm.toString("y");
    qDebug() << "             M: " << tm.toString("M");
    qDebug() << "             d: " << tm.toString("d");
    qDebug() << "   day of week: " << tm.date().dayOfWeek();
    qDebug() << "             h: " << tm.toString("h");
    qDebug() << "             H: " << tm.toString("H");
    qDebug() << "             m: " << tm.toString("m");
    qDebug() << "             s: " << tm.toString("s");
    //QLocale::setDefault(src);

    return 0;
}
