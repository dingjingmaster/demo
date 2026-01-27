/*************************************************************************
> FileName: hook-readdir-utils.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年01月27日 星期二 17时28分06秒
 ************************************************************************/
#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#ifndef RTLD_NEXT
#define RTLD_NEXT -1
#endif

void* hook_readdir(void* dirP)
{
    bool retOK = false;
    DIR* dddir = (DIR*) dirP;
    struct dirent* dirent = NULL;

    struct dirent* (*origReaddir) (DIR*) = dlsym(RTLD_NEXT, "readdir");

#if 1
    while (NULL != (dirent = origReaddir(dddir))) {
        if (0 != strncmp(dirent->d_name, "dm-", 3)) {
            retOK = true;
            break;
        }
    }
    
    if (!retOK) {
        dirent = NULL;
    }
#else
    dirent = origReaddir(dddir);
#endif

    return dirent;
}
