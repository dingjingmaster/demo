/*************************************************************************
> FileName: glib-sink.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 09 Jan 2024 04:14:31 PM CST
 ************************************************************************/
#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

// g_object_ref_sink 没看到有什么卵用...
int main (void)
{
    GFile* file = g_file_new_for_path("/tmp/aaa-test.log");
    g_object_ref_sink (file);

    //g_object_ref(file);
    //g_object_ref(file);
    //g_object_ref(file);
    g_object_ref(file);
    g_object_ref(file);
    g_object_ref_sink (file);


    g_object_unref(file);
    g_object_unref(file);
    g_object_unref(file);
    g_object_unref(file);

    printf ("%s\n", g_file_peek_path(file));

    return 0;
}
