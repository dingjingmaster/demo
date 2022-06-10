/*************************************************************************
> FileName: gio-application.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月18日 星期一 14时44分43秒
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>

static void activate (GApplication *application)
{
    /**
     * 当在这里执行一个返回到主循环持续时间较长的操作时候，
     * 应该使用 g_application_hold() 和 g_application_release() 使应用程序保持 active，
     * 直到操作完成
     */
    g_print ("activated\n");
}

static void open (GApplication* application, GFile** files, gint n_files, const gchar* hint)
{
    gint i;

    for (i = 0; i < n_files; i++) {
        gchar *uri = g_file_get_uri (files[i]);
        g_print ("open %s\n", uri);
        g_free (uri);
    }

    /**
     * 当在这里执行一个返回到主循环持续时间较长的操作时候，
     * 应该使用 g_application_hold() 和 g_application_release() 使应用程序保持 active，
     * 直到操作完成
     */
}

int main (int argc, char **argv)
{
    GApplication *app;
    int status;

    app = g_application_new ("org.gtk.TestApplication", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    g_signal_connect (app, "open", G_CALLBACK (open), NULL);
    g_application_set_inactivity_timeout (app, 10000);

    status = g_application_run (app, argc, argv);

    g_object_unref (app);

    return status;
}

