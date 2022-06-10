/*************************************************************************
> FileName: vte-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月19日 星期二 10时34分14秒
 ************************************************************************/
#include <vte/vte.h>

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    GtkWidget *m = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#if GTK_CHECK_VERSION(2,91,1)
    gtk_window_set_has_resize_grip(GTK_WINDOW(m), 0);
#endif

    GtkWidget *v = vte_terminal_new();

#if VTE_CHECK_VERSION(0,25,1)
    char **a = 0;
    g_shell_parse_argv("/bin/bash", 0, &a, 0);
    //vte_terminal_fork_command_full(VTE_TERMINAL(v), 0, 0, a, 0, 0, 0, 0, 0, 0);
#else
    //vte_terminal_fork_command(VTE_TERMINAL(v), NULL, NULL, NULL, NULL, FALSE, FALSE, FALSE);
#endif
    // 0.48 开始更名
    GError*         error = NULL;
    vte_terminal_spawn_sync (VTE_TERMINAL(v), VTE_PTY_DEFAULT, "/home/", argv, NULL, G_SPAWN_DEFAULT, NULL, NULL, NULL, NULL, &error);
    if (NULL != error) {
        printf ("error:%s\n", error->message);
        g_error_free (error);
    }

    gtk_container_add(GTK_CONTAINER(m), v);

#if VTE_CHECK_VERSION(0,27,1) && GTK_CHECK_VERSION(2,91,1)
    GtkBorder *i;
    gtk_widget_style_get (v, "inner-border", &i, NULL);
    gtk_window_set_default_geometry(GTK_WINDOW(m), 80 * vte_terminal_get_char_width(VTE_TERMINAL(v)) + (i ? (i->left + i->right) : 0), 24 * vte_terminal_get_char_height(VTE_TERMINAL(v)) + (i ? (i->top + i->bottom) : 0));
#endif

    gtk_widget_show_all(m);

#ifdef NORMAL_BEHAVIOR
    g_signal_connect(m, "delete_event", gtk_main_quit, NULL);
    g_signal_connect(v, "child-exited", gtk_main_quit, NULL);
#endif

    gtk_main();

    return 0;
}
