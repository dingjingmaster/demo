/*************************************************************************
> FileName: glib-file-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 17 Mar 2023 03:56:34 PM CST
 ************************************************************************/
#include <glib.h>

#include <stdio.h>

int main ()
{
    g_autofree char* name = g_filename_display_basename ("/aaa/bbbb/ccc/ddd/eee/fff/kkk.txt");

    g_autofree char* dir = g_filename_display_name ("/aaa/bbbb/ccc/ddd/eee/fff/kkk.txt");
    printf ("%s\n", name);
    printf ("%s\n", dir);

    return 0;
}
