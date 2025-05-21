/*************************************************************************
> FileName: realpath.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月16日 星期五 17时03分09秒
 ************************************************************************/
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <path>\n", argv[0]);
        return -1;
    }

    char buf[4096] = {0};
    getcwd(buf, sizeof(buf) - 1);
    char* path = realpath(argv[1], buf);
    printf("%s\n", path ? path : "null");

    return 0;
}
