/*************************************************************************
> FileName: get-path-dir.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sun 19 Mar 2023 11:43:50 AM CST
 ************************************************************************/
#include <QDir>
#include <QString>
#include <QFileInfo>
#include <QStringList>

int main(int argc, char *argv[])
{
    QString p = "/aaa/bbb/ccc/ddd/eee/fff.txt";

    QStringList ls = p.split("/");
    ls.pop_back();

    printf("%s\n", ls.join("/").toUtf8().constData());

    QFileInfo info(p);
    printf("%s\n", info.dir().path().toUtf8().constData());


    return 0;
}
