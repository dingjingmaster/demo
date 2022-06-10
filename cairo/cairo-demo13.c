/*************************************************************************
> FileName: cairo-demo13.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月20日 星期一 10时09分51秒
 ************************************************************************/
#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

static gboolean
on_expose_event (GtkWidget * widget,
                 GdkEventExpose * event, gpointer data)
{
        cairo_t *cr;

        cr = gdk_cairo_create (widget->window);

        cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_set_line_width (cr, 1);

        cairo_rectangle (cr, 20, 20, 120, 80);
        cairo_rectangle (cr, 180, 20, 80, 80);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);

        cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_arc (cr, 330, 60, 40, 0, 2 * M_PI);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);

        cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_arc (cr, 90, 160, 40, M_PI / 4, M_PI);
        cairo_close_path (cr);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);

        cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_translate (cr, 220, 180);
        cairo_scale (cr, 1, 0.7);
        cairo_arc (cr, 0, 0, 50, 0, 2 * M_PI);
        cairo_stroke_preserve (cr);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_fill (cr);

        cairo_destroy (cr);

        return FALSE;
}


int
main (int argc, char *argv[])
{

        GtkWidget *window;
        GtkWidget *darea;

        gtk_init (&argc, &argv);

        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

        darea = gtk_drawing_area_new ();
        gtk_container_add (GTK_CONTAINER (window), darea);

        g_signal_connect (darea, "expose-event",
                          G_CALLBACK (on_expose_event), NULL);
        g_signal_connect (window, "destroy",
                          G_CALLBACK (gtk_main_quit), NULL);

        gtk_window_set_position (GTK_WINDOW (window),
                                 GTK_WIN_POS_CENTER);
        gtk_window_set_default_size (GTK_WINDOW (window), 390, 240);

        gtk_widget_show_all (window);

        gtk_main ();

        return 0;
}
