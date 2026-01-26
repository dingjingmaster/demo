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
    QString str = "*.cc|*/a/*.c|*/.git/objects/*";

    auto arr = str.split("|");
    arr.removeAll("");
    arr.removeDuplicates();
    qInfo() << arr;

    return 0;
}

