/*************************************************************************
> FileName: parse-port.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 02 Aug 2024 11:58:19 AM CST
 ************************************************************************/
#include <QDebug>
#include <QString>

#include <stdlib.h>
#include <arpa/inet.h>

QStringList format_str(QString& str);
QList<uint> port_list(QStringList& ls);
QList<QString> addr_list(QStringList& ls);

int main (int argc, char* argv[])
{
#define IS_PORT 0

#if IS_PORT
    QString str1 = "1|2|3|4|5|6|7|8|";
    QString str2 = "S|1|2|3|4|5|6|7|8|";
    QString str3 = "F|1|8|";
    QString str4 = "S|1|2|3|F|2|9|S|2|F|1|10|";
#else
    QString str1 = "192.168.0.1|192.168.1.1|192.168.2.1|";
    QString str2 = "S|10.1.1.1|10.2.1.1|10.3.1.1|";
    QString str3 = "F|192.168.0.1|192.168.0.250";
    QString str4 = "S|192.168.0.1|192.168.0.2|192.168.0.3|F|192.168.0.5|192.168.0.15|S|192.168.2.2|F|192.168.2.1|192.168.2.5|";
#endif

    auto f1 = format_str(str1);
    auto f2 = format_str(str2);
    auto f3 = format_str(str3);
    auto f4 = format_str(str4);

#if IS_PORT
    auto s1 = port_list(f1);
    auto s2 = port_list(f2);
    auto s3 = port_list(f3);
    auto s4 = port_list(f4);
#else
    auto s1 = addr_list(f1);
    auto s2 = addr_list(f2);
    auto s3 = addr_list(f3);
    auto s4 = addr_list(f4);
#endif

    qInfo() << "str1: " << str1 << " -- " << f1 << " -- " << s1;
    qInfo() << "str2: " << str2 << " -- " << f2 << " -- " << s2;
    qInfo() << "str3: " << str3 << " -- " << f3 << " -- " << s3;
    qInfo() << "str4: " << str4 << " -- " << f4 << " -- " << s4;

    return 0;
}

QList<QString> addr_list(QStringList& ls)
{
    QSet<QString> addrs;

    return addrs.values();
}

QList<uint> port_list(QStringList& ls)
{
    QSet<uint> ports;

    for (auto l : ls) {
        if (l.endsWith("|")) {
            l = l.chopped(1);
        }

        if (l.startsWith("|")) {
            l = l.remove(0, 1);
        }

        auto pp = l.split("|");
        if (pp[0].toUpper() == "S") {
            for (int i = 1; i < pp.size(); ++i) {
                ports.insert(pp[i].toUInt());
            }
        }
        else if (pp[0].toUpper() == "F") {
            if (pp.size() != 3) {
                qWarning() << "error: " << l;
            }
            else {
                for (uint i = pp[1].toUInt(); i <= pp[2].toUInt(); ++i) {
                    ports.insert(i);
                }
            }
        }
    }

    return ports.values();
}

QStringList format_str(QString& str)
{
    QStringList ls;

    auto ss = str.split("|");
    QStringList tmp; 
    for (auto s : ss) {
        // 默认 S 
        if (tmp.isEmpty() && (s.toUpper() != "F") && (s.toUpper() != "S")) {
            tmp += "S";
        }

        if ("F" == s.toUpper() || "S" == s.toUpper()) {
            if (!tmp.isEmpty()) {
                ls.append(tmp.join("|"));
            }
            tmp.clear();
            tmp.append(s);
        }
        else {
            tmp.append(s);
        }
    }

    if (!tmp.isEmpty()) {
        ls.append(tmp.join("|"));
    }

    return ls;
}
