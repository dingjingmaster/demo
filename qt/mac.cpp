/*************************************************************************
> FileName: mac.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 22 Aug 2024 04:26:25 PM CST
 ************************************************************************/
#include <QDebug>
#include <QNetworkInterface>

int main (void)
{
    auto interface = QNetworkInterface::allInterfaces();

    for (auto i : interface) {
        if (QNetworkInterface::Loopback == i.type() 
                || QNetworkInterface::Ppp == i.type()
                || QNetworkInterface::Slip == i.type()
                || QNetworkInterface::Virtual == i.type()) {
            continue;
        }
        else if (QNetworkInterface::Ethernet == i.type()) {
            qDebug() << "以太网:" << i.hardwareAddress();
        }
        else if (QNetworkInterface::Wifi == i.type()) {
            qDebug() << "Wifi:" << i.hardwareAddress();
        }
        else {
            auto macAddress = i.hardwareAddress();
            qDebug() << "MAC: " << macAddress;
        }
    }

    return 0;
}
