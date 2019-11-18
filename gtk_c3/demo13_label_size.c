/*************************************************************************
> FileName: demo13_label_size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月18日 星期一 15时32分27秒
 ************************************************************************/

#include <stdio.h>
#include <gtk/gtk.h>

int main (int argc, char* argv[]) 
{
    GtkWidget* window;
    GtkWidget* label;
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    GtkStyleContext *context;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

    label = gtk_label_new ("这是一个\n标签!");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    context = gtk_widget_get_style_context(window);

    gtk_css_provider_load_from_data (provider,
            "label {"
            "   min-width: 10px;"
            "   min-height: 20px}",
            -1, NULL);

    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref (provider);



    gtk_container_add(GTK_CONTAINER(window), label);

    //gtk_widget_show(label);
    //gtk_widget_show(window);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}


