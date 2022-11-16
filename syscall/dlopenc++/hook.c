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

void _ZN2aa4Test3runEv()
{
    printf ("hook - run\n");
    syslog (LOG_ERR, "SSS;");
}
