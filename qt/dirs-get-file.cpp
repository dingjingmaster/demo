/*************************************************************************
> FileName: dirs-get-file.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 15 Aug 2024 04:30:02 PM CST
 ************************************************************************/
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>


int main (int argc, char* argv[])
{
    if (argc != 2) {
        qInfo() << "请输入要遍历的文件夹";
        return 1;
    }

    QString dir = argv[1];

    QList<QString> dirs;
    QFileInfo  dfi (dir);
    dirs << dfi.absoluteFilePath();
    qInfo() << "dir: " << dir << " -- " << dfi.absoluteFilePath();

    QStringList files;
    for (int i = 0; i < dirs.size(); ++i) {
        QFileInfo fi(dirs.at(i));
        //qInfo() << dirs.at(i);
        if (fi.isDir()) {
            QDir ddir(dirs.at(i));
            auto ds = ddir.entryList();
            for (auto& j : ds) {
                if ("." == j || ".." == j) {
                    continue;
                }
                QString p = QString("%1/%2").arg(fi.absoluteFilePath(), j);
                //qInfo() << "[PATH]" << p;
                QFileInfo fii(p);
                if (fii.isDir()) {
                    qInfo() << "[DIR ] " << p;
                    dirs.append(p);
                }
                else {
                    qInfo() << "[FILE] " << fii.absoluteFilePath();
                    files << p;
                }
            }
        }
        else {
            files << fi.absoluteFilePath();
        }
    }

   // qInfo() << files;

    return 0;
}
