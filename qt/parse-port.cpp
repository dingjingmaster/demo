/*************************************************************************
> FileName: parse-port.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 02 Aug 2024 11:58:19 AM CST
 ************************************************************************/
#include <QDebug>
#include <QString>

QStringList format_str(QString& str);
QList<uint> port_list(QStringList& ls);

int main (int argc, char* argv[])
{
    QString str1 = "1|2|3|4|5|6|7|8";
    QString str2 = "S|1|2|3|4|5|6|7|8";
    QString str3 = "F|1|8";
    QString str4 = "S|1|2|3|F|2|9|S|2|F|1|10";

    auto f1 = format_str(str1);
    auto f2 = format_str(str2);
    auto f3 = format_str(str3);
    auto f4 = format_str(str4);

    auto s1 = port_list(f1);
    auto s2 = port_list(f2);
    auto s3 = port_list(f3);
    auto s4 = port_list(f4);

    qInfo() << "str1: " << str1 << " -- " << f1 << " -- " << s1;
    qInfo() << "str2: " << str2 << " -- " << f2 << " -- " << s2;
    qInfo() << "str3: " << str3 << " -- " << f3 << " -- " << s3;
    qInfo() << "str4: " << str4 << " -- " << f4 << " -- " << s4;

    return 0;
}

QList<uint> port_list(QStringList& ls)
{
    QSet<uint> ports;

    for (auto l : ls) {
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
