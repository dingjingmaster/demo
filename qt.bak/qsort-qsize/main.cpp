#include <QCoreApplication>
#include <QPoint>
#include <QDebug>

bool caseLessThan (const QPoint& p1, const QPoint& p2)
{
    if (p1.x() < p2.x() || p2.y() < p2.y()) {
        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QPoint> list;

    list << QPoint(0, 1) << QPoint(1, 2) << QPoint(0, 2);

    qDebug() << list;

    std::sort(list.begin(), list.end(), caseLessThan);

    qDebug() << list;

    return a.exec();
}
