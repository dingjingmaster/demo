/*************************************************************************
> FileName: mime-type.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月06日 星期三 15时53分52秒
 ************************************************************************/
#include <QMimeDatabase>
#include <QMimeType>
#include <QFileInfo>
#include <QDebug>


int main (int argc, char* argv[])
{
    if (argc < 2) {
        qInfo() << "Input file name";
        return 0;
    }

    QFileInfo fileInfo(argv[1]);
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(fileInfo);

    qInfo() << "MIME Type: " << mime.name();

    return 0;
}
