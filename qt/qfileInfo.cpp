/*************************************************************************
> FileName: qfileInfo.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月01日 星期五 10时34分37秒
 ************************************************************************/
#include <QDir>
#include <QDebug>
#include <QFileInfo>


int main ()
{
    QFileInfo fi("/tmp/aaa/bbb/ccc/ddd/file.txt");

    qInfo() << "baseName: " << QString("%1.%2").arg(fi.baseName()).arg(fi.completeSuffix());
    qInfo() << "baseName: " << fi.completeBaseName();
    qInfo() << "dir: " << fi.dir().absolutePath();
    return 0;
}
