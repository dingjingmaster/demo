/*************************************************************************
> FileName: hook.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 16 Nov 2022 07:52:16 PM CST
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

void _ZN5Peony17FileCopyOperation3runEv()
{
    printf ("hook - 构造\n");
}


