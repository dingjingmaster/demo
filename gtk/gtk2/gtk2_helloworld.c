/*************************************************************************
> FileName: hello_world.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月23日 星期一 21时32分53秒
 ************************************************************************/
#include <gtk/gtk.h>

void delete_cb (GtkWidget* widget, gpointer udata);
void hello_cb (GtkWidget* widget, gpointer udata);

int main (int argc, char* argv[])
{
    GtkWidget* window = NULL;
    GtkWidget* button = NULL;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (G_OBJECT(window), "delete_event", G_CALLBACK(delete_cb), NULL);
    gtk_container_set_border_width (GTK_CONTAINER(window), 10);

    button = gtk_button_new_with_label ("hello world");
    g_signal_connect (G_OBJECT(button), "clicked", G_CALLBACK(hello_cb), NULL);
    g_signal_connect_swapped (G_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), window);

    gtk_container_add (GTK_CONTAINER(window), button);

    gtk_widget_show (button);
    gtk_widget_show (window);

    gtk_main();

    return 0;
}

void delete_cb (GtkWidget* widget, gpointer udata)
{
    g_print ("delete event occurred\n");
    // TRUE 改为 FALSE 程序会关闭, FALSE 停止信号传递
    // TRUE 阻止信号的进一步传播，FALSE 让信号继续传播
    return FALSE;
}

void hello_cb (GtkWidget* widget, gpointer udata)
{
    g_print ("hello world\n");
}

