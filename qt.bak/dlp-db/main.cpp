#include <QCoreApplication>
#include <qglobal.h>

#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>
#include <QDataStream>
#include <QByteArray>

#include "sqlite-utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if 0
    SqliteUtils s;

    QString channel = "1";
    qint64 time = QDateTime::currentDateTime().toMSecsSinceEpoch() + 10000000;
    QString md5 = QString("md5_%1").arg(time);

    FileInfo f(md5, 0, time, channel);

    qDebug() << s.insert(f);
    qDebug() << s.contains(md5, SqliteUtils::CHANNEL_USB);
    qDebug() << s.contains(md5, SqliteUtils::CHANNEL_PRINTER);
#endif

    // MD5

    QFile f ("/usr/bin/bash");
    if (!f.exists()) {
        return -1;
    }

    QCryptographicHash hash (QCryptographicHash::Md5);

    if (f.open(QIODevice::ReadOnly)) {
        QDataStream in(&f);
        hash.addData(in.device());

        QByteArray res = hash.result();
        if (!res.isEmpty()) {
            qInfo() << res.toHex().toUpper();
        }
    }


    return a.exec();
}
