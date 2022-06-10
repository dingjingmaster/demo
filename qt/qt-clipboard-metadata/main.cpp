#include <QDebug>
#include <QApplication>
#include <QMimeData>
#include <QClipboard>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.connect (QApplication::clipboard (), &QClipboard::changed, [=] () {
        const QMimeData* mimeData = qApp->clipboard()->mimeData();
        qDebug() << mimeData->formats();
        qDebug() << qApp->clipboard ()->text ();
        qDebug() << mimeData->text();
        if (mimeData->hasFormat("peony-qt/is-cut")) {
            qDebug() << "==> is-cut? :" << mimeData->data("peony-qt/is-cut");
        } 

        qDebug() << "\n\n\n\n\n\n\n";
    });

    a.connect (QApplication::clipboard (), &QClipboard::findBufferChanged, [=] () {
        qDebug() << "find buffer changed";
    });

    a.connect (QApplication::clipboard (), &QClipboard::selectionChanged, [=] () {
        qDebug() << "selection changed";
    });

    return a.exec();
}
