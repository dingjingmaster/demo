#include <QCoreApplication>
#include <qglobal.h>

#include <QDebug>
#include <QDateTime>

#include "sqlite-utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqliteUtils s;

    QString channel = "1";
    qint64 time = QDateTime::currentDateTime().toMSecsSinceEpoch() + 10000000;
    QString md5 = QString("md5_%1").arg(time);

    FileInfo f(md5, 0, time, channel);

    qDebug() << s.insert(f);
    qDebug() << s.contains(md5, SqliteUtils::CHANNEL_USB);
    qDebug() << s.contains(md5, SqliteUtils::CHANNEL_PRINTER);


    return a.exec();
}
