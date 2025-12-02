/*************************************************************************
> FileName: glib-str-hash.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年12月02日 星期二 17时49分53秒
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

int main() 
{
    const char* str1 = "Hello GLib Hash2";
    const char* str2 = "Hello GLib Hash1";

    printf ("hash: %s -> %lu\n", str1, g_str_hash(str1));
    printf ("hash: %s -> %lu\n", str1, g_str_hash(str2));


    return 0;
}
