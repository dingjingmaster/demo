/*************************************************************************
> FileName: gio-source.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 24 Mar 2023 08:26:34 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>

/* 检查事件源是否有事件发生，如果有事件已经发生，则无需轮询（poll） */
gboolean g_source_myidle_prepare(GSource *source, gint *timeout) 
{
    *timeout = 0;
    printf("prepare\n");
    return TRUE;
}

/* 检查事件源是否有事件发生 */
gboolean g_source_myidle_check(GSource *source) 
{
    printf("check\n");
    return TRUE;
}

/* 分发事件源的事件=>调用事件源的回调函数 */
gboolean g_source_myidle_dispatch(GSource *source, GSourceFunc callback, gpointer user_data) 
{
    gboolean again;
    printf("dispatch\n");

    /* g_source_set_callback未设置 */
    if (!callback) {
        g_warning("no callback");
        return FALSE;
    }

    again = callback(user_data);

    return again;
}

/* source被销毁前调用，使用此函数释放资源 */
void g_source_myidle_finalize(GSource *source) 
{
    printf("finalize\n");
    return ;
}

gboolean myidle(gchar *message) 
{
    g_print("%s\n", message);

    /*
     * G_SOURCE_REMOVE, 删除事件源
     * G_SOURCE_CONTINUE, 保留事件源
     */
    return G_SOURCE_REMOVE;
}


int main(int argc, char const *argv[]) 
{
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GMainContext *context = g_main_loop_get_context(loop);

    GSourceFuncs g_source_myidle_funcs = {
        g_source_myidle_prepare,
        g_source_myidle_check,
        g_source_myidle_dispatch,
        g_source_myidle_finalize,
    };

    /* 创建新事件源实例，传入了事件的函数表、事件结构体大小 */
    GSource *source = g_source_new(&g_source_myidle_funcs, sizeof(GSourceFuncs));
    /* 设置新事件源source的回调函数 */
    g_source_set_callback(source, (GSourceFunc)myidle, "Hello, world!", NULL);
    /* source关联特定的GMainContext对象 */
    g_source_attach(source, context);
    g_source_unref(source);

    g_main_loop_run(loop);

    g_main_context_unref(context);
    g_main_loop_unref(loop);

    return 0;
}
