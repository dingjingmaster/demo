#include "clipboard-utils.h"

#include <unistd.h>

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClipboardUtils cu;
    qDebug() << cu.getUrlsByX11 ();

#if 0
    while (1) {
        getchar ();
        qDebug() << "start get uri======>";
        qDebug() << "===>" << cu.getUrlsByX11 ();
        qDebug() << "finished!";

    }
#endif

    return a.exec();
}
