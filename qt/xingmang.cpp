/*************************************************************************
> FileName: xingmaang.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月07日 星期五 09时36分13秒
 ************************************************************************/

#include <QApplication>
#include "./xingmang/StarburstWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StarburstWidget w;
    w.setWindowTitle("2D 星芒图案 - Qt C++");
    w.resize(80, 80);
    w.show();

    return app.exec();
}
