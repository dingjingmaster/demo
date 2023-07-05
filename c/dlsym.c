/*************************************************************************
> FileName: dlsym.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 27 Jun 2023 10:27:56 AM CST
 ************************************************************************/
#include <syslog.h>
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>

int main (void)
{
    int (*ptr) (const char*, int, ...) = dlsym (RTLD_NEXT, "open");
    if (NULL == ptr) {
        printf ("not found: open!\n");
        return -1;
    }
    else {
        printf ("found: open!\n");
    }

    return 0;
}
