/*************************************************************************
> FileName: shared-pointer.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 20 Mar 2023 01:26:15 PM CST
 ************************************************************************/
#include <QApplication>
#include <QSharedPointer>
#include <QWidget>


int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto w = new QWidget(nullptr);

    auto kk = QSharedPointer<QWidget> (w);

    return app.exec();
}
