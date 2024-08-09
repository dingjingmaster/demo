#include <QCoreApplication>

#include <QWebSocket>

#include <iostream>
#include <libwebsockets.h>


#if 0
static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        std::cout << "Connection established" << std::endl;
        break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
        std::cout << "Received message: " << std::string((char*)in, len) << std::endl;
        break;
    case LWS_CALLBACK_CLIENT_WRITEABLE: {
        const char *msg = "Hello Server!";
        unsigned char buf[LWS_PRE + 512];
        unsigned char *p = &buf[LWS_PRE];
        size_t n = snprintf((char*)p, 512, "%s", msg);
        lws_write(wsi, p, n, LWS_WRITE_TEXT);
        break;
    }
    case LWS_CALLBACK_CLOSED:
        std::cout << "Connection closed" << std::endl;
        break;
    default:
        break;
    }
    return 0;
}

int main()
{
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    struct lws_protocols protocols[] = {
        { "default", callback_websocket, 0, 512, },
        { NULL, NULL, 0, 0 }, // 结束标志
    };

    info.port = CONTEXT_PORT_NO_LISTEN; // 不监听端口
    info.protocols = protocols;

    struct lws_context *context = lws_create_context(&info);
    if (context == NULL) {
        std::cerr << "Failed to create lws context" << std::endl;
        return -1;
    }

    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = context;
    ccinfo.address = "ws://192.168.2.101";
    ccinfo.port = 9099;
    ccinfo.path = "/socket.io/";
    ccinfo.host = lws_canonical_hostname(context);
    ccinfo.origin = "origin";
    ccinfo.protocol = protocols[0].name;
    ccinfo.ietf_version_or_minus_one = -1;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if (wsi == NULL) {
        std::cerr << "Failed to connect to server" << std::endl;
        lws_context_destroy(context);
        return -1;
    }

    while (lws_service(context, 1000) >= 0) {
        // 继续处理事件循环
    }

    lws_context_destroy(context);

    return 0;
}

#else

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
    auto xDevice = "WINDOW;f3640c23-72e4-435f-bfae-c1c753af4010;AND_GUARD_LINUX;3.0.0";
    auto xToken = "0d2ce3838c2cdfa2a3cdae61992cdbd105ec6373b9a7d9ccf21c064641e5e96563df615df8a7f038f518afa6ee04e8344c481989eb800823";

    req.setRawHeader("X-Device", xDevice);
    req.setRawHeader("X-Access-Token", xToken);
    req.setRawHeader("Cookie", QString("X-Access-Token=%1;X-Device=%2").arg(xToken, xDevice).toUtf8());

    auto url = QString("ws://192.168.2.101:9099/socket.io/?X-Access-Token=%1&X-Device=%2&EIO=%3&transport=websocket&t=P4s0HgM").arg(xToken, xDevice, "3");
    // auto url = QString("ws://192.168.2.101:9099/socket.io");//.arg(xToken, xDevice);
    // auto url = QString("ws://192.168.2.101:9099/socket.io");


    QObject::connect(&w, &QWebSocket::aboutToClose, [&] () {
        qInfo() << "aboutToClose" << "\n";
    });

    QObject::connect(&w, &QWebSocket::binaryFrameReceived, [=] (const QByteArray& frame, bool isLastFrame) {
        qInfo() << "binaryFrameReceived" << "\n";
    });

    QObject::connect(&w, &QWebSocket::binaryMessageReceived, [=] (const QByteArray& message) {
        qInfo() << "binarymessageReceived" << "\n";
    });

    QObject::connect(&w, &QWebSocket::textFrameReceived, [=] (const QString& frame, bool isLastFrame) {
        qInfo() << "textFrameReceived" << "\n";
    });

    QObject::connect(&w, &QWebSocket::textMessageReceived, [=] (const QString& message) {
        qInfo() << "textMessageReceived\n" << message << "\n";
    });

    QObject::connect(&w, &QWebSocket::disconnected, [&] () {
        qInfo() << "disconnected: " << "\n";
    });

    QObject::connect(&w, &QWebSocket::connected, [=] () {
        qInfo() << "connected" << "\n";
    });

    QObject::connect(&w, &QWebSocket::pong, [=] (quint64 elapsedTime, const QByteArray& payload) {
        qInfo() << "pong" << "\n";
    });

    QObject::connect(&w, &QWebSocket::sslErrors, [=] (const QList<QSslError>& error) {
        qInfo() << "sslErrors" << "\n";
    });

    QObject::connect(&w, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [=] (QAbstractSocket::SocketError error) {
        qDebug() << "error: " << error;
    });

    QObject::connect(&w, &QWebSocket::stateChanged, [&] (QAbstractSocket::SocketState state) {
        switch (state) {
        case QAbstractSocket::ClosingState:
        case QAbstractSocket::UnconnectedState: {
            qDebug() << "unconnected!";
            w.open(url);
            break;
        }
        case QAbstractSocket::HostLookupState: {
            qDebug() << "host lookup state!";
            break;
        }
        case QAbstractSocket::ConnectingState: {
            qDebug() << "connecting!";
            break;
        }
        case QAbstractSocket::ConnectedState: {
            qDebug() << "connected!";
            break;
        }
        default: {
            qDebug() << "other states";
        }
        }
    });

    qDebug() << url;
    req.setUrl(QUrl(url));

    w.open(url);
    // w.open(req);

    return a.exec();
}

#endif
