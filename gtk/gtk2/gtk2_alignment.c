/*************************************************************************
> FileName: gtk2_alignment.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 11时05分50秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *alignment;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkAlignment");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);

    button = gtk_button_new_with_label("Press");
    alignment = gtk_alignment_new(1, 0.3, 0.5, 0.9);

    gtk_container_add(GTK_CONTAINER(alignment), button);
    gtk_container_add(GTK_CONTAINER(window), alignment);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
