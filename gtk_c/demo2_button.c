#include <gtk/gtk.h>

static void print_hello (GtkWidget* w, gpointer d) {
    g_print("hello word\n");
}

static void activate (GtkApplication* app, gpointer data) {
    GtkWidget* window = NULL;
    GtkWidget* button = NULL;
    GtkWidget* button_box = NULL;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "标题");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    /* gtk_button_box_new 创建一个 button_box 输入 GtkOrientation 枚举参数，设置布局为水平、垂直 */
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    /* 将 button_box 添加到 window 窗口中 */
    gtk_container_add(GTK_CONTAINER(window), button_box);

    /* 创建按钮 */
    button = gtk_button_new_with_label("标签");
    /* 关联按钮点击事件和一个回调函数 */
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    /* 销毁窗体 */
    g_signal_connect_swapped (button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    /* 将按钮添加到 button_box */
    gtk_container_add(GTK_CONTAINER(button_box), button);
    /* 展示所有窗体 */
    gtk_widget_show_all(window);
}

int main(int argc, char* argv[]) {
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk_examplt", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
