/*************************************************************************
> FileName: ffs.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 26 Aug 2024 03:13:21 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    printf ("%d\n", ffs(8));
    printf ("%d\n", ffs(4096));
    printf ("%d\n", 204799 >> (ffs(8) - 1));

    return 0;
}
