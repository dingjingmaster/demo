/*************************************************************************
> FileName: qstring.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月27日 星期五 10时43分33秒
 ************************************************************************/
#include <QDebug>
#include <QString>

int main (void)
{
    const QString str1 = "你好";

    qInfo() << str1.size();
    qInfo() << str1.toUtf8().size();

    return 0;
}

