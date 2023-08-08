/*************************************************************************
> FileName: md5.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 08 Aug 2023 04:59:03 PM CST
 ************************************************************************/
#include <stdio.h>

#include <QFile>
#include <QCryptographicHash>


int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要计算 md5 的文件路径\n");
        return -1;
    }

    QFile f(argv[1]);
    if (!f.exists() || !f.open(QIODevice::ReadOnly)) {
        printf ("打开文件: %s 失败\n", argv[1]);
        return -1;
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(&f);
    QString ss = hash.result().toHex();
    printf ("md5: %s\n", ss.toUtf8().toStdString().c_str());
}
