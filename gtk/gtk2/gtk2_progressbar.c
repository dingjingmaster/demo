/*************************************************************************
> FileName: gtk2_progressbar.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 11时26分20秒
 ************************************************************************/

#include <gtk/gtk.h>

gboolean timeout_callback(gpointer data) 
{
    gdouble value;
    GString *text;

    value = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(data));

    value += 0.01;
    if(value > 1.0) {
        value = 0.0;
    }

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data), value);

    text = g_string_new(gtk_progress_bar_get_text(GTK_PROGRESS_BAR(data)));
    g_string_sprintf(text, "%d %%", (int) (value * 100));

    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(data), text->str);

    return TRUE;
}

int main(int   argc, char *argv[]) 
{
    gint timer;
    GtkWidget *window;
    GtkWidget *progressBar;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkProgressBar");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 30);

    progressBar = gtk_progress_bar_new();
    timer = gtk_timeout_add(100, timeout_callback, progressBar);

    gtk_container_add(GTK_CONTAINER(window), progressBar);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
                                                     
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
