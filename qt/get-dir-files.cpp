#include <QDir>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QStringList>

QStringList get_dir_files(const QString &dir)
{
    QStringList files;
    QStringList dirs;
    dirs << dir;

    for (int i = 0; i < dirs.size(); ++i) {
        QFileInfo fi(dirs.at(i));
        if (fi.isDir()) {
            QDir ddir(dirs.at(i));
            auto ds = ddir.entryList();
            for (auto& j : ds) {
                if ("." == j || ".." == j) {
                    continue;
                }
                QString p = QString("%1/%2").arg(fi.absoluteFilePath(), j);
                QFileInfo fii(p);
                if (fii.isDir()) {
                    dirs.append(p);
                }
                else {
                    files << p;
                }
            }
        }
        else {
            files << fi.absoluteFilePath();
        }
    }

    return files;
}


int main (int argc, char* argv[])
{
    if (argc < 2) {
        qInfo() << "请输入要遍历的文件夹!";
        return -1;
    }

    qInfo() << get_dir_files(argv[1]);
}
