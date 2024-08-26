/*************************************************************************
> FileName: calc-left-move.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 26 Aug 2024 02:32:18 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    if (argc != 3) {
        printf("输入要左移的数字、左移位数\n");
        return -1;
    }

    int64_t input = strtol(argv[1], NULL, 10);
    int64_t bits = strtol(argv[2], NULL, 10);

    printf ("%d << %d = %d\n", input, bits, input << bits);

    return 0;
}

