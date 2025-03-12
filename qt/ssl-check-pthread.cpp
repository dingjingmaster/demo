/*************************************************************************
> FileName: ssl-check.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月10日 星期一 13时42分42秒
 ************************************************************************/
#include <glib.h>
#include <QDebug>
#include <pthread.h>
#include <QSslSocket>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QNetworkAccessManager>


void testSslVersion(QSsl::SslProtocol pro, const QString& name, const QUrl& url)
{
        QSslSocket socket;
        socket.setProtocol(pro);
        socket.connectToHostEncrypted(url.host(), url.port());
        if (socket.waitForConnected(5000)) {
                qInfo() << name << "支持";
                socket.disconnectFromHost();
        }
        else {
                qInfo() << name << "不支持";
        }
}

void* https_thread (void* urlT)
{
	QEventLoop ev;
	qDebug() << "SSL Support               : " << QSslSocket::supportsSsl();
	qDebug() << "SSL Library               : " << QSslSocket::sslLibraryVersionString();
	qDebug() << "SSL Library Build Version : " << QSslSocket::sslLibraryBuildVersionString();

	QNetworkAccessManager manager;
	QUrl url((char*) urlT);
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);
	QNetworkReply* reply = manager.get(request);

        qInfo() << "支持的SSL/TLS版本：";
        testSslVersion(QSsl::TlsV1_0, "TLSv1.0", url);
        testSslVersion(QSsl::TlsV1_1, "TLSv1.1", url);
        testSslVersion(QSsl::TlsV1_2, "TLSv1.2", url);
        testSslVersion(QSsl::TlsV1_3, "TLSv1.3", url);
        qInfo() << "---";
        testSslVersion(QSsl::AnyProtocol, "AnyProtocol", url);
        testSslVersion(QSsl::SecureProtocols, "SecureProtocols", url);

	QObject::connect(reply, &QNetworkReply::finished, [&] () {
                qWarning() << "ret: " << reply->error() << " -- " << reply->errorString() << " STATUS: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); 
		reply->deleteLater();
		ev.quit();
	});

	ev.exec();
	return NULL;
}


int main (int argc, char* argv[])
{
	if (argc < 2) {
		qDebug() << "请输入要访问的https链接\n";
		return -1;
	}

	QCoreApplication app(argc, argv);

	pthread_t p1 = 0;
	pthread_create(&p1, NULL, https_thread, argv[1]);
	pthread_join(p1, NULL);

	return app.exec();
}
