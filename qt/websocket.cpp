/*************************************************************************
> FileName: websocket.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Aug 2024 07:00:57 PM CST
 ************************************************************************/
#include <QWebSocket>
#include <QApplication>

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);

    QWebSocket web("ws://");

    return app.exec();
}
