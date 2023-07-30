/*************************************************************************
> FileName: qserver.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 24 Mar 2023 10:07:14 AM CST
 ************************************************************************/
#include <QApplication>
#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>

#define DSM_FILE_CONTROL_SERVER_SOCKET_PATH "/tmp/aa.sock"


int main (int argc, char* argv[])
{
    //QApplication app(argc, argv);

    QLocalServer* server = new QLocalServer(nullptr);

    server->setSocketOptions (QLocalServer::WorldAccessOption);

    QLocalServer::removeServer (DSM_FILE_CONTROL_SERVER_SOCKET_PATH);

    server->listen(DSM_FILE_CONTROL_SERVER_SOCKET_PATH);
#if 0
    QObject::connect(server, SIGNAL(newConnection()), [=] () {
        qDebug() << "a";
    });
#endif

#if 1

    server->waitForNewConnection(-1);
    QLocalSocket* sock = server->nextPendingConnection();
    if (sock) {
        while (true) {
            qDebug() << "start read!";
            QTextStream in(sock);
            qDebug() << in.readAll();
        }
    }
    else {
        qDebug() << "ok" << sock->errorString();
    }
    delete sock;
#endif

    //return app.exec();
    return 0;
}
