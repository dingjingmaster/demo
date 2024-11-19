#include <QDir>
#include <QDebug>


int main (void)
{
    QString dir = "/aa/bb/cc/dd/ee/ff/hh/ii/kk.c";

    do {
        qDebug() << dir;
        int lastIdx = dir.lastIndexOf('/');
        if (!lastIdx || "/" == dir) {
            break;
        }
        dir = dir.left(lastIdx);
    } while(true);
}
