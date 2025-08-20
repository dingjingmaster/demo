/*************************************************************************
> FileName: hook-redirect.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月20日 星期三 17时46分54秒
 ************************************************************************/
#include <dlfcn.h>
#include <stdio.h>
//#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>


int open (const char* pathName, int flags, int mode)
{
    static int (*realOpen) (const char*, int, int) = NULL;
    if (!realOpen) {
        realOpen = dlsym(RTLD_NEXT, "open");
    }

    syslog(LOG_ERR, "%d write file: %s", getpid(), pathName);

    return realOpen(pathName, flags, mode);
}

int dup2 (int oldFd, int newFd)
{
    static int (*realDup2) (int, int) = NULL;
    if (!realDup2) {
        realDup2 = dlsym(RTLD_NEXT, "dup2");
    }

    syslog(LOG_ERR, "dup2: %d", getpid());

    return realDup2(oldFd, newFd);
}
