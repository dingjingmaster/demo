/*************************************************************************
> FileName: gtk2_paned_h.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 10时31分48秒
 ************************************************************************/

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *hPaned;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkPaned");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    hPaned = gtk_hpaned_new();

    gtk_paned_pack1(GTK_PANED(hPaned), 
    gtk_frame_new("GtkPaned Add1 Here"), FALSE, FALSE);
    gtk_paned_pack2(GTK_PANED(hPaned), 
    gtk_text_view_new(), TRUE, FALSE);

    gtk_container_add(GTK_CONTAINER(window), hPaned);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
