/*************************************************************************
> FileName: glib-g_intern_static_string.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 14 Apr 2023 05:34:49 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    gint a = 0;

    printf ("GTypeInfo: %s\n", g_intern_static_string("GTypeInfo"));
    printf ("GT|ypeInfo: %s\n", g_intern_static_string("GT|ypeInfo"));
    printf ("GT+ypeInfo: %s\n", g_intern_static_string("GT+ypeInfo"));
    printf ("GT_ypeInfo: %s\n", g_intern_static_string("GT_ypeInfo"));
}
