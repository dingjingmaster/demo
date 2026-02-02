/*************************************************************************
> FileName: file-ext.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年02月02日 星期一 08时52分15秒
 ************************************************************************/
#include <stdio.h>

#include <QFile>
#include <QDebug>
#include <QString>
#include <QFileInfo>


QString getFileExtName(const QString& filePath)
{
    int idx = 0;
    int extLen = 0;
    QString ext = "";
    QString name = "";
    QFileInfo fi(filePath);


    name = fi.fileName();
    idx = name.indexOf(".");
    extLen = name.count() - idx;

    if (idx > 0 && extLen > 0 && extLen < name.count()) {

        ext = name.right(name.count() - idx);
    }
    else {
        ext = fi.suffix();
    }
    return ext;
}


int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("%s <file path>\n", argv[0]);
        return 0;
    }

    qInfo() << "file: " << argv[1] << " --> " << getFileExtName(argv[1]);

    return 0;
}
