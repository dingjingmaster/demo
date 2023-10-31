/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 31 Oct 2023 05:02:24 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "spawn-window.h"

int main (int argc, char* argv[])
{
    if (argc > 1) {
        spawnWindow (111);
    }
    else {
        spawnWindow (0);
    }

    return 0;
}
