/*************************************************************************
> FileName: glib-check-int.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 09:37:39 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>


int main (int argc, char* argv[])
{
    guint dest = 0;

    // 检测相加的结果是否越界 g_uint_checked_add(dest, a, b);
    gboolean ret = g_uint_checked_add (&dest, 999999999, 999999999);
    printf ("%d -- %s\n", dest, ret ? "true" : "false");

    ret = g_uint_checked_add (&dest, 999999999, 9999999999999999999);
    printf ("%d -- %s\n", dest, ret ? "true" : "false");

    return 0;
}
