#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QHostInfo>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        qInfo() << "请输入要检测的ip!\n";
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        QHostInfo info = QHostInfo::fromName(argv[i]);
        if (info.error() == QHostInfo::NoError) {
            for (auto& addr : info.addresses()) {
                qInfo() << "Host: " << argv[i] << "  " << "IP: " << addr.toString();
            }
        }
        else {
            qInfo() << "Not found!";
        }
    }

    return 0;
}
