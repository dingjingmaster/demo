/*************************************************************************
> FileName: gtk2_timer_idle.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时29分37秒
 ************************************************************************/
        
#include <gtk/gtk.h>

gboolean idle_callback(gpointer data) 
{
    g_print("%s。。XD\n", data);
    return TRUE;
}

void button_pressed(GtkButton *button, gpointer data) 
{
    int i;
    for(i = 0; i < 10; i++) {
        g_print("%s...\n", data);
        sleep(1);
    }
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTimer");
    gtk_window_set_default_size(GTK_WINDOW(window), 150, 50);

    button = gtk_button_new_with_label("按我");

    gtk_container_add(GTK_CONTAINER(window), button);
                                        
    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(button_pressed), "do something");

    g_idle_add((GSourceFunc) idle_callback, "無事可作");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
