#include <QRegExp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QString str5 = "asdasd - 副本(1).txt";

    QRegExp regx(QString("\\ -\\ %1\\(\\d+\\)(\\.[0-9a-zA-Z]+|)$").arg("副本"));
    qDebug() << regx.indexIn(str5) << "   " << regx.cap(0);

#if 0
    QString str1 = "asdsad(1).txt";
    QString str2 = "bbbb(2)kkk(1)";
    QString str3 = "bbbb(2)kkk(1).txt";
    QString str4 = "bbbb(2)kkk";

    QRegExp regx("\\(\\d+\\)(\\.[0-9a-zA-Z]+|)$");
    qDebug() << regx.indexIn(str1) << "   " << regx.cap(0);
    qDebug() << regx.indexIn(str2) << "   " << regx.cap(0);
    qDebug() << regx.indexIn(str3) << "   " << regx.cap(0);
    qDebug() << regx.indexIn(str4) << "   " << regx.cap(0);
#endif

    Q_UNUSED(argc)
    Q_UNUSED(argv)

    return 0;
}
