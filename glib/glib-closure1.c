/*************************************************************************
> FileName: glib-closure1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 12 Jan 2023 11:22:35 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <glib-object.h>

static gboolean quit_callback (gpointer udata)
{
    GMainLoop* loop = udata;
    sleep (1);

    g_main_loop_quit (loop);

    return TRUE;
}

/**
 * 1. 在所有GLib和GTK+程序中，GMainLoop 管理所有来自 GSource 的事件
 * 2. 使用 g_source_attach(GSource*, GMainLoop*) 将二者关联
 */
int main (int argc, char* argv[])
{
    // 创建新的空闲 源，可以产生事件...
    GSource* src = g_idle_source_new ();

    GMainLoop* loop = g_main_loop_new (NULL, FALSE);

    // 创建闭包 GClosure* g_cclosure_new (GCallback cf, gpointer udata, GClosureNotify dd);
    GClosure* c = g_cclosure_new (quit_callback, loop, NULL);
    
    // 给 GSource 设置回调函数
    g_source_set_closure (src, c);

    g_source_attach (src, NULL);

    g_main_loop_run (loop);

    if (src) g_source_destroy (src);
    if (loop) g_main_loop_unref (loop);

    if (src) {g_source_unref (src);};

    return 0;
}

