/*************************************************************************
> FileName: gf-standard-path.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月23日 星期三 21时13分00秒
 ************************************************************************/
#include <QStandardPaths>
#include <QDebug>

int main ()
{
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    return 0;
}


