/*************************************************************************
> FileName: http-get.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sat 10 Aug 2024 02:25:49 PM CST
 ************************************************************************/
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto manager = new QNetworkAccessManager();
    QNetworkRequest req;
    
    QUrl url("http://192.168.2.203:12121/download?file=/fileserver/data/uploads/2024/08/09/andsec_1.0.1_amd64.deb");
    req.setUrl(url);

    QString cred = QString("%1:%2").arg("openrestyuser", "iandsec.com");
    QString hd = "Basic " + cred.toLocal8Bit().toBase64();

    req.setRawHeader("Authorization", hd.toLocal8Bit());

    auto reply = manager->get(req);
    QObject::connect(reply, &QNetworkReply::finished, [&] () {
        if (reply->error() == QNetworkReply::NoError) {
            qInfo() << "HTTP GET request successful.";
            char buf[1024] = {0};
            qint64 d = 0;
            qint64 all = 0;
            QFile f("./data.deb");
            f.open(QIODevice::ReadWrite | QIODevice::Truncate);
            while ((d = reply->read(buf, sizeof(buf))) > 0) {
                f.write(buf, d);
                all += d;
                qInfo() << "write: " << all;
            }
            f.close();
            qInfo() << d;
            //qInfo() << ((d == 0) ? "OK" : "Error");
        }
        else {
            qInfo() << reply->errorString();
        }
        reply->deleteLater();
        manager->deleteLater();
        app.quit();
    });


    return app.exec();
}
