/*************************************************************************
> FileName: round-up.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月17日 星期二 14时31分46秒
 ************************************************************************/
#include <stdio.h>

int main (void)
{
#define C_ROUND_UP(num, unit) (((num) + (unit) - 1) & ~((unit) - 1))

    printf ("1, %d\n", C_ROUND_UP(1, 16));
    printf ("2, %d\n", C_ROUND_UP(2, 16));
    printf ("9, %d\n", C_ROUND_UP(9, 16));

    return 0;
}
