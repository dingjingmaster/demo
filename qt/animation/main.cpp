#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE (animation);
    QApplication a(argc, argv);

    Window w;

    w.resize(400, 400);
    w.show();

    return a.exec();
}
