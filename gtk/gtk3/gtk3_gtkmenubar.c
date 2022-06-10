/*************************************************************************
> FileName: gtkmenubar.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月10日 星期二 10时15分22秒
 ************************************************************************/

#include <gtk/gtk.h>

gint on_button_press_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    g_return_val_if_fail(widget != NULL, FALSE);
    g_return_val_if_fail(GTK_IS_MENU(data), FALSE);
    g_return_val_if_fail(event != NULL, FALSE);

    if (event->type == GDK_BUTTON_PRESS)
    {
        GdkEventButton *mouse = (GdkEventButton *) event;
        if (mouse->button == GDK_BUTTON_SECONDARY)
        {
            gtk_widget_show_all(GTK_WIDGET(data));
            // gtk_menu_popup_at_pointer(GTK_MENU(data), event);
            gtk_menu_popup(GTK_MENU(data), NULL, NULL, NULL, NULL, mouse->button, mouse->time);
            return TRUE;
        }
    }
    return FALSE;
}

int main( int argc, char *argv[])
{
    GtkWidget *window;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
    GtkWidget *label = gtk_label_new("test");
    gtk_container_add(GTK_CONTAINER(window), label);

    GtkWidget *menu = gtk_menu_new();
    GtkWidget *pasteMi = gtk_menu_item_new_with_label("Paste");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), pasteMi);
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press_event), menu);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
