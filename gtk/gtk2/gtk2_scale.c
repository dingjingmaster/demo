/*************************************************************************
> FileName: gtk2_scale.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 13时35分20秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *scale;
    
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkScale");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 30);

    scale = gtk_hscale_new_with_range(0.0, 100.0, 1.0);
    gtk_scale_set_digits(GTK_SCALE(scale), 0);
    gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_TOP);

    gtk_container_add(GTK_CONTAINER(window), scale);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}
