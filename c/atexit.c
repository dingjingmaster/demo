/*************************************************************************
> FileName: atexit.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月04日 星期三 14时32分34秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

static void cb1 (void)
{
    printf("callback1\n");
}

static void cb2 (void)
{
    printf("callback2\n");
}

int main (void)
{
    atexit(cb1);
    atexit(cb2);

    printf("main exit\n");

    return 0;
}
