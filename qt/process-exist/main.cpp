#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QProcess pro;

    pro.setProgram ("pidof");
    pro.setArguments (QStringList() << "nautilus");

    pro.start ();
    pro.waitForFinished (-1);
    if (pro.readAllStandardOutput ().toInt () > 1) {
        qDebug() << "exists";
    } else {
        qDebug() << "not exists";
    }

    return a.exec();
}
