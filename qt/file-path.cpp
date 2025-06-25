/*************************************************************************
> FileName: file-path.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月25日 星期三 11时56分40秒
 ************************************************************************/
#include <QFile>
#include <QDebug>


int main (int argc, char* argv[])
{
    if (argc != 2) {
        qInfo() << "Usage: " << argv[0] << " <path>";
        return 0;
    }

    QFile file(argv[1]);

    qInfo() << "file name: " << file.fileName();

    return 0;
}
