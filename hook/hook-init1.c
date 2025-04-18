/*************************************************************************
> FileName: hook-init1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月18日 星期五 09时25分13秒
 ************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>

void hook_init() __attribute__((constructor()));

typedef int (*OpenPointer)(const char *pathname, int flags, ...);
int open (const char *pathname, int flags, ...)
{
	    syslog(LOG_ERR, "open IN, file: '%s'", pathname);

    int mode = 0;

    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    OpenPointer oldOpen = (OpenPointer) dlsym ((void*) -1, "open");
    if (oldOpen == NULL) {
        printf("dlsym open error\n");
        return -1;
    }

    if (mode == 0) {
        oldOpen(pathname, flags);
    } else {
        oldOpen(pathname, flags, mode);
    }

    errno = EACCES;

    return -1;
}

void hook_init()
{
	setenv("TEST", "111111", 1);
	printf("hook init, open: %p\n", dlsym((void*)-1, "open"));
}
