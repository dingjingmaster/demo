/*************************************************************************
> FileName: gtk2_aspect_frame.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 11时18分26秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkAspectFrame");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    frame = gtk_aspect_frame_new("caterpillar", 0.5, 0.5, 2, FALSE);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);

    button = gtk_button_new_with_label("Press");
    gtk_container_add(GTK_CONTAINER(frame), button);
    gtk_container_add(GTK_CONTAINER(window), frame);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
