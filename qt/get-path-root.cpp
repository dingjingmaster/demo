/*************************************************************************
> FileName: get-path-root.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月30日 星期二 15时40分17秒
 ************************************************************************/
#include <QDebug>
#include <QString>

#include <stdlib.h>

QString get_root_path(const QString& path) 
{
    if (!path.startsWith("/")) {
        return "";
    }

    QStringList arr = path.split("/");
    if (arr.length() <= 2) {
        return path;
    }

    QStringList ls = arr.mid(0, 2);


    return ls.join("/");
}


int main (void)
{
    const char* paths[] = {
        "",
        "/",
        "/a",
        "/a/b",
        "/a/b/c",
        "/a/b/c/d",
        "/a/b/c/d/",
        NULL
    };

    for (int i = 0; paths[i]; ++i) {
        qInfo() << paths[i] << " -- " << get_root_path(paths[i]) << "\n";
    }

    return 0;
}
