/*************************************************************************
> FileName: gtk2_scrolled_window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 10时56分19秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *image;
    GtkWidget *scrolledWindow;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkScrolledWindow");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);

    image = gtk_image_new_from_file("resource/1.png");
    scrolledWindow = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledWindow), image);

    gtk_container_add(GTK_CONTAINER(window), scrolledWindow);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
