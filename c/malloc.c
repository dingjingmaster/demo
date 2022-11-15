/*************************************************************************
> FileName: malloc.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 08 Nov 2022 04:06:55 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

void main (int argc, char* argv[])
{
    char* a = malloc(1024 * 1024 * 66 * 10);
    printf ("%s\n", a ? "ok" : "error");

    return 0;
}
