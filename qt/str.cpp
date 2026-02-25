/*************************************************************************
> FileName: str.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年02月10日 星期二 19时48分21秒
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

    auto arr = QString(argv[1]).mid(2, -1);
    qInfo() << argv[1] << " -- " << arr;

    return 0;
}

