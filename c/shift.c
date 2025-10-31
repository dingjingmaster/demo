/*************************************************************************
> FileName: shift.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月28日 星期二 19时05分46秒
 ************************************************************************/
#include <stdio.h>

int main (void)
{
    const int size = 4099;

    printf ("block: %d\n", 1 << 12);
    printf ("num: %d\n", size >> 12);
    printf ("yu: %d\n", size & ~((1 << 12) - 1));

    return 0;
}
