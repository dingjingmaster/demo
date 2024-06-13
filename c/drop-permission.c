/*************************************************************************
> FileName: drop-permission.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 13 Jun 2024 01:24:28 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main (void)
{
    if (setgid(getgid()) < 0) {
        printf ("err1\n");
        return -1;
    }

    if (setuid(getuid()) < 0) {
        printf ("err2\n");
        return -2;
    }

    return 0;
}
