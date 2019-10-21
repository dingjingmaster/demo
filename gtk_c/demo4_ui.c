#include <gtk/gtk.h>

static void print_hello(GtkWidget w, gpointer p) {
    g_print ("hello word\n");
}


int main (int argc, char* argv[]) {
    GtkBuilder* builder = NULL;
    GObject* window = NULL;
    GObject* button = NULL;
    GError* error = NULL;

    gtk_init(&argc, &argv);

    // 创建 builder 并装入 ui 文件
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "builder.ui", &error) == 0) {
        g_printerr("装入文件错误: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    window = gtk_builder_get_object(builder, "window");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    button = gtk_builder_get_object(builder, "button1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "button2");

    return 0;
}
