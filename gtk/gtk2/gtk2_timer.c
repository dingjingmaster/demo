/*************************************************************************
> FileName: glib_timer.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时17分50秒
 ************************************************************************/

#include <gtk/gtk.h>

void button_pressed(GtkButton *button, GTimer *timer) 
{
    static gdouble elapsedTime;
    static gboolean isRunning = FALSE;
                
    if(isRunning) {
        elapsedTime = g_timer_elapsed(timer, NULL);
        gtk_button_set_label(button, "Start");
        g_print("Elapsed Time: %f s\n", elapsedTime);
    } else {
        g_timer_start(timer);
        gtk_button_set_label(button, "Stop");
    }

    isRunning = !isRunning;
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;
    GTimer *timer;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTimer");
    gtk_window_set_default_size(GTK_WINDOW(window), 150, 50);

    button = gtk_button_new_with_label("Start");

    gtk_container_add(GTK_CONTAINER(window), button);
                                            
    timer = g_timer_new();

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(button_pressed), timer);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
