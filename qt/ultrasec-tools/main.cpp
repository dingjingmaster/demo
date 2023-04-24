#include <QCoreApplication>
#include <qglobal.h>

#include <QUrl>
#include <QTimer>
#include <QSettings>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QCommandLineParser>

QNetworkReply* requestInclueQByteA(QNetworkRequest &req, QByteArray b, QNetworkAccessManager *manager)
{
    QByteArray d;
    QNetworkReply *reply = manager->post(req, d);

    QEventLoop loop;

    QTimer     timer;
    timer.singleShot(20 * 1000, &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    if (reply != NULL && !reply->isFinished()) {
        if(reply) {
            reply->deleteLater();
        }
        return NULL;
    }

    return reply;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray  data;

    QString ip = "";
    QString port = "";
    QString clientid = "";
    QString servicecode = "/InteractiveServer/s/rs/uni";


    QCommandLineParser parser;
    const QCommandLineOption help = parser.addHelpOption();

    QCommandLineOption ipC("i", "请输入IP", "ip", "");
    QCommandLineOption portC("p", "请输入端口", "port", "");
    QCommandLineOption clientidC("c", "请输入clientid", "clientid", "");

    parser.addOption(ipC);
    parser.addOption(portC);
    parser.addOption(clientidC);

    parser.process(a);

    ip = parser.value(ipC);
    port = parser.value(portC);
    clientid = parser.value(clientidC);

    QSettings s("/usr/local/ultrasec/server.ini", QSettings::IniFormat);

    if (ip.isEmpty() || "" == ip) {
        ip = s.value("server/ip").toString();
    }

    if (port.isEmpty() || "" == port) {
        port = s.value("server/port").toString();
    }

    if (clientid.isEmpty() || "" == clientid) {
        clientid = s.value("local/clientid").toString();
    }

    QUrl url = QString("https://%1:%2%3").arg(ip, port, servicecode);

    qDebug() << url.toDisplayString();

    QNetworkAccessManager manager;

    QNetworkRequest req = QNetworkRequest( url );

    QSslConfiguration mSslConfig = QSslConfiguration::defaultConfiguration();
    mSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    mSslConfig.setProtocol(QSsl::AnyProtocol);              // QSsl::TlsV1
    req.setSslConfiguration(mSslConfig);

    req.setAttribute( QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Automatic );

    req.setRawHeader( "method~name", "getAllTask");

    req.setRawHeader( "user~clientId",  clientid.toStdString().c_str() );

    QNetworkReply  *reply = requestInclueQByteA( req, data, &manager );
    if ( reply == NULL ) {
        qDebug() << "reply is null";
        return 101;
    }

    if ( reply->error() ) {
        qDebug() << "reply->error : " << reply->error();
        reply->deleteLater();
        return 102;
    }

    QString returnFlag = reply->rawHeader( "data~returnFlag" );

    if (0 == returnFlag.toInt() ) {
        qDebug() << "return 0";
    }

    return 0;
}
