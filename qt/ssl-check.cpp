/*************************************************************************
> FileName: ssl-check.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月10日 星期一 13时42分42秒
 ************************************************************************/
#include <QDebug>
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

int main (int argc, char* argv[])
{
	if (argc < 2) {
		qDebug() << "请输入要访问的https链接\n";
		return -1;
	}

	QCoreApplication app(argc, argv);

	qInfo() << "SSL Support               : " << QSslSocket::supportsSsl();
	qInfo() << "SSL Library               : " << QSslSocket::sslLibraryVersionString();
	qInfo() << "SSL Library Build Version : " << QSslSocket::sslLibraryBuildVersionString();

	QUrl url(argv[1]);
	qInfo() << "支持的SSL/TLS版本：";
	testSslVersion(QSsl::TlsV1_0, "TLSv1.0", url);
	testSslVersion(QSsl::TlsV1_1, "TLSv1.1", url);
	testSslVersion(QSsl::TlsV1_2, "TLSv1.2", url);
	testSslVersion(QSsl::TlsV1_3, "TLSv1.3", url);
	qInfo() << "---";
	testSslVersion(QSsl::AnyProtocol, "AnyProtocol", url);
	testSslVersion(QSsl::SecureProtocols, "SecureProtocols", url);

#if 0
	auto protocols = QSslSocket::supportedSslVersions();
	for (auto& s : protocols) {
		switch (s) {
			case QSsl::SslV3: qInfo() << "SSLv3"; break;
			case QSsl::TlsV1_0: qInfo() << "TLSv1.0"; break;
			case QSsl::TlsV1_1: qInfo() << "TLSv1.1"; break;
			case QSsl::TlsV1_2: qInfo() << "TLSv1.2"; break;
			case QSsl::TlsV1_3: qInfo() << "TLSv1.3"; break;
			case QSsl::AnyProtocol: qInfo() << "AnyProtocol"; break;
			case QSsl::SecureProtocols: qInfo() << "SecureProtocols"; break;
			default: qInfo() << "Unknown"; break;
		}
	}
#endif

	QNetworkAccessManager manager;
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
	QNetworkReply* reply = manager.get(request);

	QObject::connect(reply, &QNetworkReply::finished, [&] () {
		qDebug() << "Reply received: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		reply->deleteLater();
		app.quit();
	});

	return app.exec();
}
