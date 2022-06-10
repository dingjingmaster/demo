/*************************************************************************
> FileName: gtk2_ruler.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 13时47分00秒
 ************************************************************************/

#include <gtk/gtk.h>

#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *hrule;
    GtkWidget *vrule;

    gtk_init (&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "GtkRuler");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
                                        
    gtk_widget_set_events(window, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
                                            
    table = gtk_table_new (2, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(window), table);

    hrule = gtk_hruler_new();
    gtk_ruler_set_metric(GTK_RULER(hrule), GTK_PIXELS);
    gtk_ruler_set_range(GTK_RULER(hrule), 7, 13, 0, 20);                   
    g_signal_connect_swapped(G_OBJECT(window), "motion_notify_event", G_CALLBACK(EVENT_METHOD(hrule, motion_notify_event)), hrule);
    gtk_table_attach(GTK_TABLE(table), hrule, 1, 2, 0, 1, GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_FILL, 0, 0);

    vrule = gtk_vruler_new();
    gtk_ruler_set_metric(GTK_RULER(vrule), GTK_PIXELS);
    gtk_ruler_set_range(GTK_RULER(vrule), 0, 400, 10, 400);
    g_signal_connect_swapped(G_OBJECT(window), "motion_notify_event", G_CALLBACK(EVENT_METHOD(vrule, motion_notify_event)), vrule);
    gtk_table_attach(GTK_TABLE(table), vrule, 0, 1, 1, 2, GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main(); 

    return 0;
}
