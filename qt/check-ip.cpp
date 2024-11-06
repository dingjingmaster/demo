#include <QDebug>
#include <QRegExp>
#include <QString>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        qInfo() << "请输入要检测的ip!\n";
        return -1;
    }

    QRegExp rxp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    for (int i = 1; i < argc; ++i) {
        qInfo() << argv[i] << "  " << (rxp.exactMatch(argv[i]) ? "是Ipv4" : "不是Ipv4");
    }

    return 0;
}
