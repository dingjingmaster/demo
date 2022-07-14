/*************************************************************************
> FileName: test.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 14 Jul 2022 03:29:49 PM CST
 ************************************************************************/
#include <syscall.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    unsigned long ret = syscall(__NR_perf_event_open, NULL, 0, 0, 0, 0);
    printf ("%d\n", (int) ret);

    return 0;
}
