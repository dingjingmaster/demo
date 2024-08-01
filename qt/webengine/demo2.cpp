/*************************************************************************
> FileName: demo2.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 01 Aug 2024 01:45:51 PM CST
 ************************************************************************/
#include <QApplication>
#include <QWebEngineView>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto view = new QWebEngineView(nullptr);
    view->load(QUrl("http://www.qt.io/"));
    view->show();

    return app.exec();
}
