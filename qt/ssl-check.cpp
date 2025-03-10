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

int main (int argc, char* argv[])
{
	if (argc < 2) {
		qDebug() << "请输入要访问的https链接\n";
		return -1;
	}

	QCoreApplication app(argc, argv);

	qDebug() << "SSL Support               : " << QSslSocket::supportsSsl();
	qDebug() << "SSL Library               : " << QSslSocket::sslLibraryVersionString();
	qDebug() << "SSL Library Build Version : " << QSslSocket::sslLibraryBuildVersionString();

	QNetworkAccessManager manager;
	QUrl url(argv[1]);
	QNetworkRequest request(url);
	QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
	QNetworkReply* reply = manager.get(request);

	QObject::connect(reply, &QNetworkReply::finished, [&] () {
		qDebug() << "Reply received: " << reply->readAll();
		reply->deleteLater();
		app.quit();
	});

	return app.exec();
}
