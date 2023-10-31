/*************************************************************************
> FileName: file.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 30 Oct 2023 04:22:21 PM CST
 ************************************************************************/
#include <stdio.h>

#include <QFile>
#include <QDebug>
#include <QProcess>


int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入文件路径\n");
        return -1;
    }

    QFile f(argv[1]);
    if (!f.exists()) {
        printf ("打开文件: %s 失败\n", argv[1]);
        return -1;
    }

    QProcess proc;
    proc.setProgram("file");
    proc.setArguments(QStringList() << argv[1]);
    proc.start(QProcess::ReadOnly);
    proc.waitForFinished(-1);

    auto res = proc.readAllStandardOutput();

    qDebug() << res;
}
