/*************************************************************************
> FileName: gtk2_handle_box.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 11时21分16秒
 ************************************************************************/

     

#include <gtk/gtk.h>

int main(int argc, char *argv[]) 
{
    GtkWidget *vbox;
    GtkWidget *window;
    GtkWidget *handleBox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkHandleBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    vbox = gtk_vbox_new(FALSE, 5);

    handleBox = gtk_handle_box_new();
    gtk_handle_box_set_handle_position( GTK_HANDLE_BOX(handleBox), GTK_POS_LEFT);
    gtk_container_add(GTK_CONTAINER(handleBox), gtk_label_new("\n選單位置\n"));

    gtk_box_pack_start(GTK_BOX(vbox), handleBox, FALSE, FALSE,  5);
    gtk_box_pack_start(GTK_BOX(vbox), gtk_text_view_new(), TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
