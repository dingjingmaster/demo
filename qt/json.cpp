/*************************************************************************
> FileName: json.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 07:26:42 PM CST
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QRegExp>
#include <QJsonDocument>

int main ()
{
    QString str = "0{\"sid\":\"57f01bb4-72e8-4a39-b553-6565aa91d063\",\"upgrades\":[\"websocket\"],\"pingInterval\":25000,\"pingTimeout\":60000}";

    QRegExp digit("^\\d+");
    QRegExp c("\\");

    str.replace(digit, "");
    //str.replace(c, "");

    qInfo() << str;

    QByteArray arr = QByteArray::fromRawData(str.toUtf8(), str.size());

    auto json = QJsonDocument::fromJson(str.toUtf8());

    qInfo() << json;

    return 0;
}
