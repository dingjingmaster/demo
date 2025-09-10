/*************************************************************************
> FileName: regex-anda.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月01日 星期二 18时14分02秒
 ************************************************************************/
#include <QDebug>
#include <QRegExp>

int main ()
{
    QString str1 = "aaa.h";
    QString str2 = "aaa.c";
    QString str3 = "aaa.sh";
    QString str4 = "aaa.cc";
    QString str5 = "aaa.txt";
    QString str6 = "aaa.java";
    QString str7 = "aaa.javaas";
    QString str8 = "aaa.txtasda";
    QString str9 = "aaa.ccoasdasd";
    QString str10 = "aaa.shasdd2wd";
    QString str11 = "aaa.casdasdasda";
    QString str12 = "aaa.hasdadasdas";

    //QRegExp regex ("(.h|.c|.cc|.txt|.java)$");
    QRegExp regex ("(\\.h$|\\.c$|\\.cc$|\\.txt$|\\.java$)");

    qDebug() << str1 << "    " << regex.indexIn(str1) << "    " << regex.cap(0);
    qDebug() << str2 << "    " << regex.indexIn(str2) << "    " << regex.cap(0);
    qDebug() << str3 << "    " << regex.indexIn(str3) << "    " << regex.cap(0);
    qDebug() << str4 << "    " << regex.indexIn(str4) << "    " << regex.cap(0);
    qDebug() << str5 << "    " << regex.indexIn(str5) << "    " << regex.cap(0);
    qDebug() << str6 << "    " << regex.indexIn(str6) << "    " << regex.cap(0);
    qDebug() << str7 << "    " << regex.indexIn(str7) << "    " << regex.cap(0);
    qDebug() << str8 << "    " << regex.indexIn(str8) << "    " << regex.cap(0);
    qDebug() << str9 << "    " << regex.indexIn(str9) << "    " << regex.cap(0);
    qDebug() << str10 << "    " << regex.indexIn(str10) << "    " << regex.cap(0);
    qDebug() << str11 << "    " << regex.indexIn(str11) << "    " << regex.cap(0);
    qDebug() << str12 << "    " << regex.indexIn(str12) << "    " << regex.cap(0);

    return 0;
}
