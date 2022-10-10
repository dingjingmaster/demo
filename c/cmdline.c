/*************************************************************************
> FileName: cmdline.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 10 Oct 2022 11:01:57 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入要解析命令行的进程id\n");
    }

    int pid = atoi(argv[1]);

    char buf[32] = {0};

    snprintf(buf, sizeof(buf) - 1, "/proc/%d/cmdline", pid);
    printf ("%s\n", buf);
    if (!access(buf)) {
        printf ("%s not exists!\n", buf);
        return -1;
    }

    FILE* fr = fopen (buf, "rb");
    if (fr) {
        char tmp[2048] = {0};
        fread (tmp, 1, sizeof(tmp) - 1, fr);

        int i = 0;
        for (i = 0; tmp[i] != 0; ++i);

        printf ("tmp:%s\n", tmp + ++i);
        fclose (fr);
    }

    return 0;
}
