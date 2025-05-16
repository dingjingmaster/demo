/*************************************************************************
> FileName: glib-get-parent.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月15日 星期四 16时07分04秒
 ************************************************************************/
#include <glib.h>
#include <gio/gio.h>

#include <stdio.h>

int main (int argc, char* argv[])
{
    if (argc < 2 || argv[1][0] != '/') {
        printf ("Input abs path\n");
        return 1;
    }

    GFile* f = g_file_new_for_path(argv[1]);
    if (f) {
        GFile* fp = g_file_get_parent(f);
        if (fp) {
            g_autofree char* fff = g_file_get_path(fp);
            if (fff) {
                printf("parent file: %s\n", fff);
            }
            else {
                printf ("parent file is null\n");
            }
        }
        else {
            printf ("gfile is null\n");
        }
    }
    else {
        printf ("error\n");
    }

    return 0;
}
