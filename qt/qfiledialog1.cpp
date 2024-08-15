#include <cstdio>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFiles);

    QStringList files;
    dlg.connect (&dlg, &QFileDialog::filesSelected, [&] (const QStringList& fs) {
        files = fs;
    });

    dlg.exec();

    qDebug() << files;

    QJsonArray arr;
    for (auto& f : files) {
        QJsonObject obj;
        obj["file"] = f;
        arr.append(obj);
    }

    qDebug() << QJsonDocument(arr).toJson();

    return 0;
}
