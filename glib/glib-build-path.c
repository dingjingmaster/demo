/*************************************************************************
> FileName: glib-build-path.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Oct 2022 04:38:02 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    g_autofree char* path = g_build_path("/", "aa", "bb", "cc", "dd", NULL);
    printf("%s\n", path);

    g_autofree char* parentPath = g_path_get_dirname("/aa/bb/cc/dd");
    printf("%s\n", parentPath);

    return 0;
}

