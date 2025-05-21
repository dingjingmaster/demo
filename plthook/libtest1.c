/*************************************************************************
> FileName: libtest1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 17时56分16秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>

int test_libc2(int a, int b)
{
    int ret = open("/ttest/libc2.so", O_RDONLY);
    printf ("ret: %d\n", ret);

    return a+b;
}
