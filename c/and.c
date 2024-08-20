/*************************************************************************
> FileName: and.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Aug 2024 10:05:39 AM CST
 ************************************************************************/
#include <stdio.h>

int main (void)
{
    printf ("%d\n", 1 & 1);
    printf ("%d\n", 2 & 2);
    printf ("%d\n", 4 & 4);
    printf ("%d\n", 8 & 8);
    
    printf ("%d\n", 1 & 0); // 0x0001 & 0x0000
    printf ("%d\n", 2 & 1); // 0x0010 & 0x0001
    printf ("%d\n", 4 & 3); // 0x0100 & 0x0011
    printf ("%d\n", 8 & 7); // 0x1000 & 0x0111

    return 0;
}
