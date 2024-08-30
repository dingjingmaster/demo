/*************************************************************************
> FileName: system-language.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 30 Aug 2024 09:32:13 AM CST
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QLocale>

int main (int argc, char* argv[])
{
    qInfo() << QLocale::languageToString(QLocale().language());

    return 0;
}
