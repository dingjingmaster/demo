/*************************************************************************
> FileName: file-server.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月16日 星期三 13时53分30秒
 ************************************************************************/
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonObject>
#include <QStorageInfo>
#include <QApplication>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QNetworkCookie>
#include <QHttpMultiPart>
#include <QNetworkRequest>
#include <QNetworkInterface>
#include <QNetworkAccessManager>


int main (int argc, char* argv[])
{
	QApplication app(argc, argv);

	const QString url = "http://192.168.2.203:12121//backUpFileRename";
	const QString filePath = "/home/dingjing/hh-2.c";
	const QString newFilePath = "/home/dingjing/hh-3.c";

	QJsonObject js;

	js["timestamp"] = QDateTime::currentDateTime().toSecsSinceEpoch();
    js["loginUserName"] = "dingjing";
    js["filePath"] = QString("%1/%2").arg("dingjing", filePath);
	js["newFileName"] = QString("%1/%2").arg("dingjing", newFilePath);

	const QByteArray content = QJsonDocument(js).toJson(QJsonDocument::Compact);

    qInfo() << "url: " << url;

	QByteArray resp;
    QHttpPart textPart;
    QNetworkRequest req;
    QNetworkAccessManager http;
    QHttpMultiPart multiPart(QHttpMultiPart::FormDataType);

    req.setUrl(url);
    req.setRawHeader("content", content);
    req.setRawHeader("Accept", "application/json, text/plain, */*");
    const auto fff = filePath.split("/");

    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data;filename=\"%1\"").arg(fff.back())));
    textPart.setBody(content);
    multiPart.append(textPart);

    QString user = "openrestyuser"; //Storage::getInstance()->getFileServerUser();
    QString passwd = "iandsec.com"; //Storage::getInstance()->getFileServerPasswd();
    auto verify = QString("%1:%2").arg(user, passwd);
    QString header = "Basic " + verify.toLocal8Bit().toBase64();
    req.setRawHeader("Authorization", header.toLocal8Bit());

    req.setRawHeader("Accept-Language", "zh-CN");

//    auto deviceId = QString("%1;%2;AND_GUARD_%3;%4").arg(CLIENT_OS_TYPE,s->getClientId(),CLIENT_OS_TYPE, s->getClientVersion());
//    req.setRawHeader("X-Device", deviceId.toUtf8());
//    req.setRawHeader("X-Access-Token", "");

    QEventLoop loop;
    QTimer timer;

    timer.singleShot(3000, &loop, &QEventLoop::quit);
    http.connect(&http, &QNetworkAccessManager::finished, [&] (QNetworkReply* reply) -> void {
        if (QNetworkReply::NoError == reply->error()) {
            resp = reply->readAll();
        }
        else {
            qWarning() << "error: " << reply->errorString();
        }
        loop.exit(0);
    });

    http.post(req, &multiPart);

    timer.start();
    loop.exec();

	qInfo() << resp;

	return 0;
}
