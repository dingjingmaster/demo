/*************************************************************************
> FileName: gtk2_frame.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 11时08分09秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *frame;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkFrame");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 150);

    frame = gtk_frame_new("caterpillar");
    gtk_frame_set_label_align(GTK_FRAME(frame), 0.5, 0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);

    gtk_container_add(GTK_CONTAINER(window), frame);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
