/*************************************************************************
> FileName: gtk3_transparent.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月17日 星期五 15时51分19秒
 ************************************************************************/
#include <gtk/gtk.h>
#include <gdk/gdkscreen.h>
#include <cairo.h>

static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer user_data);
static gboolean expose_draw(GtkWidget *widget, GdkEventExpose *event, gpointer userdata);
static void clicked(GtkWindow *window, GdkEventButton *event, gpointer user_data);

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Alpha Demo");
    g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);

    gtk_widget_set_app_paintable(window, TRUE);

    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(expose_draw), NULL);
    g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(screen_changed), NULL);

    //gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    //gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
    //g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(clicked), NULL);

    //GtkWidget* fixed_container = gtk_fixed_new();
    //gtk_container_add(GTK_CONTAINER(window), fixed_container);
    //GtkWidget* button = gtk_button_new_with_label("button1");
    //gtk_widget_set_size_request(button, 100, 100);
    //gtk_container_add(GTK_CONTAINER(fixed_container), button);

    screen_changed(window, NULL, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}


gboolean supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata) {
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if (!visual) {
        printf("Your screen does not support alpha channels!\n");
        visual = gdk_screen_get_system_visual(screen);
        supports_alpha = FALSE;
    } else {
        printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

        gtk_widget_set_visual(widget, visual);
}

static gboolean expose_draw(GtkWidget *widget, GdkEventExpose *event, gpointer userdata) {
    cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(widget));

    if (supports_alpha) {
        printf("setting transparent window\n");
        cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); 
    } else {
        printf("setting opaque window\n");
        cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); 
    }

    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);

    cairo_destroy(cr);

    return FALSE;
}

static void clicked(GtkWindow *window, GdkEventButton *event, gpointer user_data) {
    /* toggle window manager frames */
    gtk_window_set_decorated(window, !gtk_window_get_decorated(window));
}
