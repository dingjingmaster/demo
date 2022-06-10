/*************************************************************************
> FileName: gtk2_arrow.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 13时40分19秒
 ************************************************************************/

/**
 * 上下左右的按钮
 */

#include <gtk/gtk.h>

GtkWidget *create_gtk_arrow(GtkArrowType arrow_type, GtkShadowType shadow_type)
{
    GtkWidget *arrow;
    GtkWidget *button;
                
    arrow = gtk_arrow_new(arrow_type, shadow_type);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), arrow);

    return button;
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *table;
    GtkWidget *button;

    gtk_init (&argc, &argv);
                        
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "GtkArrow");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 150);

    table = gtk_table_new(3, 3, TRUE);
                                        
    gtk_table_attach_defaults(GTK_TABLE(table), create_gtk_arrow(GTK_ARROW_UP, GTK_SHADOW_IN), 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), create_gtk_arrow(GTK_ARROW_LEFT, GTK_SHADOW_OUT), 0, 1, 1, 2);            
    gtk_table_attach_defaults(GTK_TABLE(table), create_gtk_arrow(GTK_ARROW_DOWN, GTK_SHADOW_ETCHED_IN), 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), create_gtk_arrow(GTK_ARROW_RIGHT, GTK_SHADOW_ETCHED_OUT), 2, 3, 1, 2);
                                                      
    gtk_container_add(GTK_CONTAINER (window), table);
                                                            
    g_signal_connect(G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show_all(window);
                                                                    
    gtk_main();

    return 0;
}
