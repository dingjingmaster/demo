/*************************************************************************
> FileName: header.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 04 Aug 2023 11:20:16 AM CST
 ************************************************************************/
#include <QApplication>
#include "header/main-header.h"

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainHeader mh;

    mh.show();

    return app.exec();
}
