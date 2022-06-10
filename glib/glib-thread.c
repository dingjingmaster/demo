/*************************************************************************
> FileName: glib-thread.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 15时01分06秒
 ************************************************************************/

#include <gtk/gtk.h>

gpointer signal_thread(gpointer arg) 
{
    int i;
    for(i = 0; i < 5; i++) {
        g_usleep(1000000); // 暫停一秒
        g_signal_emit_by_name(arg, "clicked");
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

    gtk_widget_show(window);
    gtk_widget_show(button);

    if(!g_thread_supported()) {
        g_thread_init(NULL);
    }

    g_thread_create(signal_thread, button, FALSE, NULL);

    gtk_main();

    return 0;
}
