/*************************************************************************
> FileName: hook-readdir.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年01月27日 星期二 17时26分20秒
 ************************************************************************/
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdint.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>

#if 0
void* opendir (const char* name)
{
    if (name && (0 == strncmp(name, "/sys/block", 11))) {
        return NULL;
    }

    syslog(LOG_ERR, "%s", name);
    void* (*origOpendir) (const char* name) = dlsym(RTLD_NEXT, "opendir");

    return origOpendir(name);
}
#endif

extern void* hook_readdir(void* dirP);
void* readdir (void* dirP)
{
    return hook_readdir(dirP);
}
