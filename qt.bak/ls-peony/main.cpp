#include "dialog.h"

#include <QApplication>
#include <QDateTime>

int main(int argc, char *argv[]) {
    PeonyApplication::peony_start_time = QDateTime::currentMSecsSinceEpoch();
//    qInstallMessageHandler(messageOutput);
    qDebug() << "peony start in main time:" <<PeonyApplication::peony_start_time ;

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    PeonyApplication app(argc, argv, "peony-qt");
    if (app.isSecondary()) {
        return 0;
    }

    return app.exec();
}
