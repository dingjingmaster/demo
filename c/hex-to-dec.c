/*************************************************************************
> FileName: hex-to-dec.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 26 Aug 2024 02:27:08 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("输入要转换的数字\n");
        return -1;
    }

    int64_t input = strtol(argv[1], NULL, 16);

    printf ("0x%x -- %d\n", input, input);

    return 0;
}

