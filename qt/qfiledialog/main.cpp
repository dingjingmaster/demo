#include "dialog.h"

#include <QIcon>
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (QIcon::fromTheme("folder").isNull()) {
        QIcon::setThemeName("ukui-icon-theme-default");
        if (QStyleFactory::keys().contains("gtk2")) {
            a.setStyle("gtk2");
        }
    }

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Dialog w;
    w.show();
    return a.exec();
}
