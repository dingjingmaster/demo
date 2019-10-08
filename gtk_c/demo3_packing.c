#include <gtk/gtk.h>

static void print_hello (GtkWidget* w, gpointer d) {
    g_print("hello word\n");
}

static void activate (GtkApplication* app, gpointer d) {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;

    /* 创建新的窗体并设置标题 */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "窗体");
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    /* 创建一个容器，用来打包button */
    grid = gtk_grid_new();

    /* 容器添加到窗体 */
    gtk_container_add(GTK_CONTAINER(window), grid);

    /* gtk_grid_attach(GTK_GRID(grid), 控件, 左边, 顶部, 长, 高) */
    button = gtk_button_new_with_label("按钮1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label("按钮2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    button = gtk_button_new_with_label("退出");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

    gtk_widget_show_all(window);

}

int main(int argc, char* argv[]) {
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.demo", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
