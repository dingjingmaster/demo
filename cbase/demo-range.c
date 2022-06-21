/*************************************************************************
> FileName: demo-range.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 11:22:15 AM CST
 ************************************************************************/
#include <stdio.h>

int main ()
{
    int i = 5;

    printf ("%d ", i);

    {
        int i;
        i = 10;
        printf ("%d ", i);
    }

    printf ("%d\n", i);

    return 0;
}
