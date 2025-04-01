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
    QString str1 = "asdasd_.(2023-09-11)";
    QString str2 = "111.(2023-09-11 18_16_13)";
    QString str3 = "asdasd.tar.gz.(2023-09-11 18_16_13)";
    QString str4 = "as_.2023-09-11 18_16_13dasd.tar.gz_.(2023-09-11 18.16.13)";
    QString str5 = "as_(2023-09-11 18_16_13).dsm-dasd.tar.gz_.(2023-09-11 18_16_13)";

    QRegExp regex ("\\.\\(\\d\\d\\d\\d-\\d\\d-\\d\\d\\ \\d\\d_\\d\\d_\\d\\d\\)$");

    qDebug() << str1 << "    " << regex.indexIn(str1) << "    " << regex.cap(0);
    qDebug() << str2 << "    " << regex.indexIn(str2) << "    " << regex.cap(0);
    qDebug() << str3 << "    " << regex.indexIn(str3) << "    " << regex.cap(0);
    qDebug() << str4 << "    " << regex.indexIn(str4) << "    " << regex.cap(0);
    qDebug() << str5 << "    " << regex.indexIn(str5) << "    " << regex.cap(0);

    return 0;
}
