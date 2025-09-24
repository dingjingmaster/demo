/*************************************************************************
> FileName: ws-client.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月24日 星期三 09时44分57秒
 ************************************************************************/
#include <QCoreApplication>

#include <QWebSocket>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QWebSocket w;

    QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
    sslConf.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConf.setProtocol(QSsl::TlsV1_0OrLater);
    w.setSslConfiguration(sslConf);
    w.ignoreSslErrors();

    QNetworkProxyFactory::setUseSystemConfiguration(false);

    QNetworkRequest req;

    auto url = QString("ws://192.168.2.203:9099/socket.io/");

    QObject::connect(&w, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [&] (QAbstractSocket::SocketError error) {
        qDebug() << "error: " << error;
        a.quit();
    });

    qDebug() << url;
    req.setUrl(QUrl(url));

    w.open(url);
    // w.open(req);

    return a.exec();
}


