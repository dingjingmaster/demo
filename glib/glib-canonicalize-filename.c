/*************************************************************************
> FileName: glib-canonicalize-filename.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Oct 2022 02:58:37 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要转化的路径\n");
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        g_autofree char* path = g_canonicalize_filename(argv[i], NULL);

        printf("%s\n", path);
    }

    return 0;
}
