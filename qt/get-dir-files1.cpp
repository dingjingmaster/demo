#include <QDir>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QStringList>



int main (int argc, char* argv[])
{
    QStringList mFixedMount;
    QDir ddir("/");
    mFixedMount.clear();
    auto ds = ddir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto& j : ds) {
        if ("." == j || ".." == j) { continue; }
        if ("bin" == j || "boot" == j || "dev" == j || "efi" == j || "etc" == j
            || "lib" == j || "lib64" == j || "opt" == j || "proc" == j || "run" == j
            || "sbin" == j || "snap" == j || "srv" == j || "sys" == j || "tmp" == j)
        {
            continue;
        }
        QString p = QString("/%1").arg(j);
        QFileInfo fii(p);
        if (fii.isFile() || fii.isSymLink()) {
            continue;
        }
        mFixedMount << p;
    }

    qInfo() << "Need mount: " << mFixedMount;
}
