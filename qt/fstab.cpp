/*************************************************************************
> FileName: fstab.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年12月23日 星期二 08时30分45秒
 ************************************************************************/
#include <QFile>
#include <QDebug>
#include <QString>

int main (int argc, char* argv[])
{
    QFile fstab("/etc/fstab");

    if (!fstab.open(QFile::ReadOnly|QFile::Text)) {
        return -1;
    }

    while (!fstab.atEnd()) {
        QString line = fstab.readLine();
        line = line.trimmed();
        if (line.startsWith("#") || line.isEmpty()) {
            continue;
        }

        line.replace("\t", " ");
        while (line.contains("  ")) {
            line = line.replace("  ", " ");
        }
        qInfo() << line << "\n";
    }

    return 0;
}
