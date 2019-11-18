/*************************************************************************
> FileName: demo13_label.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月18日 星期一 15时16分46秒
 ************************************************************************/

#include <stdio.h>
#include <gtk/gtk.h>

int main (int argc, char* argv[]) 
{
    GtkWidget* window;
    GtkWidget* label;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize(window, 100, 100);
    gtk_signal_connect (GTK_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);

    label = gtk_label_new ("这是一个标签!");
    gtk_widget_set_usize (label, 10, 10);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_widget_show(label);

    gtk_container_add(GTK_CONTAINER(window), label);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

