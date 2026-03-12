/*************************************************************************
> FileName: sort.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年03月12日 星期四 18时03分54秒
 ************************************************************************/
#include <QDebug>
#include <QString>
#include <QStringList>


int main ()
{
    QStringList mm = {"/l", "/media", "/mnt/a", "/mnt"};

    qDebug() << mm << "\n";

    std::sort(mm.begin(), mm.end(), [](const QString& a, const QString& b) ->bool {
        return a.length() < b.length();
    });

    qDebug() << mm << "\n";

    return 0;
}
