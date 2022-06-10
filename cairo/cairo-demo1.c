/*************************************************************************
> FileName: cairo-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月09日 星期一 16时16分37秒
 ************************************************************************/
#include <gtk/gtk.h>
#include <cairo.h>

#define DEFAULT_WIDTH  400
#define DEFAULT_HEIGHT 200

static void paint (GtkWidget* widget, GdkEventExpose *eev, gpointer data);

gint main (gint argc, gchar **argv)
{
    GtkWidget *window;
    GtkWidget *canvas;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);

    canvas = gtk_drawing_area_new ();
    gtk_widget_set_size_request (canvas, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    g_signal_connect (G_OBJECT (canvas), "expose-event", G_CALLBACK (paint), NULL);

    gtk_container_add (GTK_CONTAINER (window), canvas);

    gtk_widget_show_all (window);

    gtk_main ();

    return 0;
}

static void paint (GtkWidget* widget, GdkEventExpose *eev, gpointer data)
{
    gint width, height;
    gint i;
    cairo_t *cr;

    width  = widget->allocation.width;
    height = widget->allocation.height;
  
    cr = gdk_cairo_create (widget->window);
  
    cairo_set_source_rgb (cr, 1,1,1);
    cairo_paint (cr);

    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    // 暂存cairo状态
    cairo_save (cr);
    cairo_set_font_size (cr, 40);

    // 写字的起点
    cairo_move_to (cr, 60, 60);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_show_text (cr, "Hello World");

    // 回到之前cairo状态
    cairo_restore (cr);
    cairo_set_source_rgb (cr, 1, 0, 0);
    cairo_set_font_size (cr, 20);
    cairo_move_to (cr, 50, 100);
    cairo_show_text (cr, "greetings from gtk and cairo");

    cairo_set_source_rgb (cr, 0, 0, 1);

    cairo_move_to (cr, 0, 150);
    for (i = 0; i < width / 10; i++) {
        cairo_rel_line_to (cr, 5,  10);
        cairo_rel_line_to (cr, 5, -10);
    }

    // 将移动轨迹画出来
    cairo_stroke (cr);
    cairo_destroy (cr);
}
