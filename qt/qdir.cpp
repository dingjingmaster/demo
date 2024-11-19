#include <QDir>
#include <QDebug>


int main (void)
{
    QString dir = "/aa/bb/cc/dd/ee/ff/hh/ii/kk.c";

    do {
        qDebug() << dir;
        if ("/" == dir) {
            break;
        }
        int lastIdx = dir.lastIndexOf('/');
        if (lastIdx <= 0) {
            dir = "/";
            continue;
        }
        dir = dir.left(lastIdx);
    } while(true);
}
