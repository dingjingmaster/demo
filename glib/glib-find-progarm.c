/*************************************************************************
> FileName: glib-find-progarm.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Oct 2022 03:03:19 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要查询命令的名字\n");
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        g_autofree char* path = g_find_program_in_path(argv[i]);

        printf("%s\n", path);
    }

    return 0;
}
