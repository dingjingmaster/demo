#include <QNetworkInterface>

int main (void)
{
    auto address = QNetworkInterface::allAddresses();
    for (int i = 0; i < address.size(); ++i) {
        auto a = address.at(i);
        if (a == QHostAddress::LocalHost
                || a == QHostAddress::LocalHostIPv6
                || a == QHostAddress::Broadcast
                || a == QHostAddress::Any) {
            continue;
        }
        if (a.isGlobal()) {
            qDebug() << "G:" << a;
        }
        else {
            qDebug() << a;
        }
    }

    auto hdAddress = QNetworkInterface().hardwareAddress();
    qDebug() << hdAddress;

    auto ipAddress = QNetworkInterface().addressEntries();
    for (int i = 0; i < ipAddress.size(); ++i) {
        qDebug() << ipAddress.at(i).ip();
    }

    return 0;
}
