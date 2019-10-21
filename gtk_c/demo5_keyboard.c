#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>

// 键盘按下事件处理函数
gboolean deal_key_press(GtkWidget* widget, GdkEventKey* event, gpointer data) {
    switch (event->keyval) {
    case GDK_KEY_Up:
        g_print("Up\n");
        break;
    case GDK_KEY_Left:
        g_print("left\n");
        break;
    case GDK_KEY_a:
        g_print("a\n");
        break;
    }
    guint key = event->keyval;
    g_print("key val = %d\n", key);

    return TRUE;
}

int main (int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    // 创建顶层窗口
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // 设置窗口的标题
    gtk_window_set_title(GTK_WINDOW(window), "key_event");

    // 设置窗口在显示器中的位置为居中
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // 设置窗口的最小大小
    gtk_widget_set_size_request(window, 400, 300);

    // "destroy" 和 gtk_main_quit 连接
    g_signal_connect(window, "destroy",
                G_CALLBACK(gtk_main_quit), NULL);

    // "key-press-event" 与 deal_key_press 连接
    g_signal_connect(window, "key-press-event",
                    G_CALLBACK(deal_key_press), NULL);

    gtk_widget_show_all(window);    // 显示窗口全部控件

    gtk_main();     // 主事件循环

    return 0;
}
