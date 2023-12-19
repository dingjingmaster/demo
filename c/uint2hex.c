/*************************************************************************
> FileName: uint2hex.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 19 Dec 2023 01:30:53 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        return -1;
    }

    unsigned long l = atol(argv[1]);

    printf ("%lu(0x%X)\n", l, l);

    return 0;
}
