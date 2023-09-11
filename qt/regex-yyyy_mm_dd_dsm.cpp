#include <QDebug>
#include <QRegExp>

int main ()
{
    QString str1 = "asdasd_(2023-09-11).dsm";
    QString str2 = "asdasd.wps_(2023-09-11).dsm";
    QString str3 = "asdasd.tar.gz_(2023-09-11).dsm";
    QString str4 = "as_(2023-09-11)dasd.tar.gz_(2023-09-11).dsm";
    QString str5 = "as_(2023-09-11).dsm-dasd.tar.gz_(2023-09-11).dsm";

    QRegExp regex ("_\\(\\d\\d\\d\\d-\\d\\d-\\d\\d\\).dsm$");

    qDebug() << str1 << "    " << regex.indexIn(str1) << "    " << regex.cap(0) << "    " << str1.replace(regex, "");
    qDebug() << str2 << "    " << regex.indexIn(str2) << "    " << regex.cap(0) << "    " << str2.replace(regex, "");
    qDebug() << str3 << "    " << regex.indexIn(str3) << "    " << regex.cap(0) << "    " << str3.replace(regex, "");
    qDebug() << str4 << "    " << regex.indexIn(str4) << "    " << regex.cap(0) << "    " << str4.replace(regex, "");
    qDebug() << str5 << "    " << regex.indexIn(str5) << "    " << regex.cap(0) << "    " << str5.replace(regex, "");

    return 0;
}
