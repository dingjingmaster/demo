/*************************************************************************
> FileName: gtk3_drag_drop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 04 Jul 2023 02:46:32 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>


gboolean label_press (GtkWidget* widget, GdkEvent* event, gpointer udata);

void drag_begin (GtkWidget* widget, GdkDragContext* context, gpointer udata);
void drag_data_get (GtkWidget* widget, GdkDragContext* context, GtkSelectionData* selection_data, guint info, guint time, gpointer udata);
void drag_data_received (GtkWidget* widget, GdkDragContext* context, gint x, gint y, GtkSelectionData* selection_data, guint info, guint time, gpointer udata);


int main (int argc, char* argv[])
{
    gtk_init (&argc, &argv);

    GtkWidget* window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    GtkWidget* label = gtk_label_new ("drag me");

    gtk_widget_set_size_request (window, 500, 300);
    gtk_widget_set_size_request (label , 400, 200);

    gtk_widget_set_can_focus (label, true);

    gtk_drag_source_set(label, GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK, NULL, 0, GDK_ACTION_COPY);
    gtk_drag_dest_set (label, GTK_DEST_DEFAULT_ALL, NULL, 0, GDK_ACTION_COPY);

    gtk_drag_source_set(window, GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK, NULL, 0, GDK_ACTION_COPY);
    gtk_drag_dest_set  (window, GTK_DEST_DEFAULT_ALL, NULL, 0, GDK_ACTION_COPY);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "button-press-event", G_CALLBACK(label_press), NULL);
    g_signal_connect(window, "drag-begin", G_CALLBACK(drag_begin), NULL);
    g_signal_connect(window, "drag-data-get", G_CALLBACK(drag_data_get), NULL);
    g_signal_connect(window, "drag-data-received", G_CALLBACK(drag_data_received), NULL);

    g_signal_connect(GTK_LABEL(label), "drag-begin", G_CALLBACK(drag_begin), NULL);
    g_signal_connect(GTK_LABEL(label), "drag-data-get", G_CALLBACK(drag_data_get), NULL);
    g_signal_connect(GTK_LABEL(label), "drag-data-received", G_CALLBACK(drag_data_received), NULL);

    g_signal_connect(GTK_LABEL(label), "button-press-event", G_CALLBACK(label_press), NULL);

    gtk_container_add(GTK_CONTAINER(window), label);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

gboolean label_press (GtkWidget* widget, GdkEvent* event, gpointer udata)
{
    g_print ("pressed\n");


    return true;
}

void drag_begin (GtkWidget* widget, GdkDragContext* context, gpointer udata)
{
    g_print("drag begin\n");
}

void drag_data_get (GtkWidget* widget, GdkDragContext* context, GtkSelectionData* selection_data, guint info, guint time, gpointer udata)
{
    g_print ("get drag\n");
    const char* text = "Hello World!";

    gtk_selection_data_set_text(selection_data, text, -1);
}

void drag_data_received (GtkWidget* widget, GdkDragContext* context, gint x, gint y, GtkSelectionData* selection_data, guint info, guint time, gpointer udata)
{
    g_print ("drag received!\n");
    gchar* text = gtk_selection_data_get_text(selection_data);
    g_print ("recived text: %s\n", text);
    g_free (text);
}
