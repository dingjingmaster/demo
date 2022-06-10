/*************************************************************************
> FileName: gio-file-size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月15日 星期一 16时37分54秒
 ************************************************************************/


int main (int argc, char **argv)
{
    int status = 0;
#if 0
    GApplication *app;

    app = g_application_new ("org.gtk.TestApplication", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    g_signal_connect (app, "open", G_CALLBACK (open), NULL);
    g_application_set_inactivity_timeout (app, 10000);

    status = g_application_run (app, argc, argv);

    g_object_unref (app);

#endif
    return status;
}

