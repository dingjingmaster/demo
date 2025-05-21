/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 16时45分10秒
 ************************************************************************/
#include <stdio.h>

extern int test_libc(int a, int b);

int main (int argc, char* argv[])
{
    printf ("%d + %d = %d\n", 1, 2, test_libc(1, 2));

    return 0;
}

