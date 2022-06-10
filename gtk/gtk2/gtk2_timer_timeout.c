/*************************************************************************
> FileName: gtk2_timer_timeout.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 14时21分50秒
 ************************************************************************/

#include <gtk/gtk.h>

gboolean timeout_callback(GtkButton *button) 
{
    static gint count = 0;
    if(count < 5) {
        g_signal_emit_by_name(button, "clicked");
        count++;
        return TRUE;
    } else {
        return FALSE;
    }
}

// 自訂Callback函式
void button_clicked(GtkWidget *button, gpointer data) 
{
    g_print("按鈕按下：%s\n", (char *) data);
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "哈囉！GTK+！");

    button = gtk_button_new_with_label("按我");
    gtk_container_add(GTK_CONTAINER(window), button);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(button_clicked), "哈囉！按鈕！");

    g_timeout_add(1000, (GSourceFunc) timeout_callback, button);

    gtk_widget_show(window);
    gtk_widget_show(button);


    gtk_main();

    return 0;
}
