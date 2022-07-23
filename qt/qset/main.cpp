#include <QCoreApplication>

#include <QSet>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSet<QString> a1;
    a1 += "aa";
    a1 += "bb";
    a1 += "cc";
    a1 += "dd";

    QSet<QString> a2;
    a2 += "cc";
    a2 += "dd";
    a2 += "ee";
    a2 += "ff";

    qDebug() << "a1: " << a1;
    qDebug() << "a2: " << a2;


    qDebug() << "a1 - a2: " << a1 - a2;
    qDebug() << "a1 + a2: " << a1 + a2;


    return a.exec();
}
