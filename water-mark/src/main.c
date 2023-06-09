/*************************************************************************
> FileName: main.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Jun 2023 09:37:36 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "x11.h"


int main (int argc, char* argv[])
{
#if 0
    int pid = fork();
    if (pid > 0) {
        exit (0);
    }
    else if (0 == pid) {
        setsid();
    }
#endif

    x11_backend_start();
}
