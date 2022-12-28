/*************************************************************************
> FileName: hook-for-kylin.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 28 Dec 2022 11:19:53 AM CST
 ************************************************************************/
#include <syslog.h>

#include <QString>
#include <QStringList>

int kylin_peony_check_copy_or_move_valid (const QStringList& srcUris, const QString& destUri, int op)
{
    syslog (LOG_ERR, "[HOOK] kylin peony hook");

    return 0;
}

