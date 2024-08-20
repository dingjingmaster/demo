/*************************************************************************
> FileName: page-size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Aug 2024 10:22:19 AM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    printf ("%d\n", sysconf(_SC_PAGESIZE));

    return 0;
}
