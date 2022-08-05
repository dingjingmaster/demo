#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTimer tim;

    tim.setInterval (1 * 1000);
    tim.connect (&tim, &QTimer::timeout, [=] () {
        qDebug() << "timeout";
    });
    tim.start();

    return a.exec();
}
