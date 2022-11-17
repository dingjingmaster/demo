/*************************************************************************
> FileName: hook.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 16 Nov 2022 11:33:38 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void _ZN2aa4TestC2Ev()
{
    printf ("hook - 构造\n");
}

typedef int (* A) ();
void _ZN2aa4Test3runEv()
{
    printf ("hook - run\n");
    syslog (LOG_ERR, "SSS;");

    void* handle1 = dlopen ("./libtest.so", RTLD_NOW);
    if (!handle1) {
        syslog (LOG_ERR, "hook test error");
        return;
    }

    A oldCopy = (A) dlsym (handle1, "_ZN2aa4Test4getAEv");
    if (!oldCopy) {
        syslog (LOG_ERR, "cannot find copy function");
        return;
    }

    syslog (LOG_ERR, "%d\n", oldCopy());
}

