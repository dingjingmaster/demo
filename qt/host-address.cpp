/*************************************************************************
> FileName: host-address.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Aug 2024 11:05:31 AM CST
 ************************************************************************/
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>

int main (int argc, char* argv[])
{
    QString curIp;

    auto address = QNetworkInterface::allAddresses();
    for (int i = 0; i < address.size(); ++i) {
        if (address.at(i) != QHostAddress::LocalHost
                && address.at(i).toIPv4Address()) {
            curIp = address.at(i).toString();
            break;
        }
    }

    qDebug() << curIp;
}
