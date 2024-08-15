/*************************************************************************
> FileName: filedialog2.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 15 Aug 2024 03:45:53 PM CST
 ************************************************************************/
#include <cstdio>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QApplication>
#include <QJsonDocument>
#include <QCryptographicHash>


int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFileDialog dlg;
    QStringList files;

    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFiles);

    dlg.connect(&dlg, &QFileDialog::filesSelected, [&] (const QStringList& fs) {
        files = fs;
    });

    int ret = dlg.exec();
    qDebug() << "ret: " << ret;
    qDebug() << files;

    QJsonArray arr;
    for (auto& f : files) {
        QJsonObject obj;
        QFile file(f);
        QFileInfo fileInfo(f);
        obj.insert("dirName", fileInfo.absoluteDir().dirName());
        obj.insert("fileName", fileInfo.fileName());
        obj.insert("fileMD5", "");
        if (file.open(QIODevice::ReadOnly)) {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(&file);
            obj.insert("fileMD5", QString(hash.result().toHex().toUpper()));
        }
        qInfo() << obj;
        arr.append(obj);
    }

    qInfo() << "arr: " << arr;

    QJsonDocument json(arr);
    QString jsStr = json.toJson(QJsonDocument::Compact);

    qInfo() << "js: " << jsStr;

    return 0;
}
