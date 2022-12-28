/*************************************************************************
> FileName: hook-for-kylin.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 28 Dec 2022 11:19:53 AM CST
 ************************************************************************/
#include <syslog.h>
#include <stdbool.h>

bool _ZN5Peony26hook_check_operation_validERK11QStringListRK7QStringNS_18_FileOperationModeE (const void const* srcUrls, const void const* destUrls, int type)
{
    syslog (LOG_ERR, "[HOOK] kylin peony hook");

    return 0;
}

