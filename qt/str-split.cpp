/*************************************************************************
> FileName: str-split.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月25日 星期三 11时06分35秒
 ************************************************************************/
#include <QDebug>
#include <QString>

#include <stdio.h>


int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("Usage: %s <string>\n", argv[0]);
        return 0;
    }

    auto arr = QString(argv[1]).split("");
    arr.removeAll("");
    qInfo() << arr;

    return 0;
}

