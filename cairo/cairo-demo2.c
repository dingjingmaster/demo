/*************************************************************************
> FileName: cairo-demo2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月09日 星期一 16时56分32秒
 ************************************************************************/
#include <gtk/gtk.h>
#include <cairo.h>

#define DEFAULT_WIDTH  400
#define DEFAULT_HEIGHT 200

typedef struct {
  gboolean active;
  gdouble  x, y;
  gdouble  w, h;
}
SelectionInfo;

static void paint (GtkWidget* widget, GdkEventExpose* eev, SelectionInfo* sel_info);
static gboolean event_press   (GtkWidget* widget, GdkEventButton* bev, SelectionInfo* sel_info);
static gboolean event_release (GtkWidget* widget, GdkEventButton* bev, SelectionInfo* sel_info);
static gboolean event_motion  (GtkWidget* widget, GdkEventMotion* mev, SelectionInfo* sel_info);

gint main (gint argc, gchar **argv)
{
    GtkWidget     *window;
    GtkWidget     *canvas;
    SelectionInfo  selection = {FALSE, 0, 0, 0, 0};

    gtk_init (&argc, &argv);

    window   = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);

    canvas = gtk_drawing_area_new ();
    gtk_widget_set_size_request (canvas, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    g_signal_connect (G_OBJECT (canvas), "expose-event", G_CALLBACK (paint), &selection); 
    gtk_widget_add_events (canvas, GDK_BUTTON1_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

    g_signal_connect (G_OBJECT (canvas), "button_press_event", G_CALLBACK (event_press), &selection);
    g_signal_connect (G_OBJECT (canvas), "button_release_event", G_CALLBACK (event_release), &selection);
    g_signal_connect (G_OBJECT (canvas), "motion_notify_event", G_CALLBACK (event_motion), &selection);
    gtk_container_add (GTK_CONTAINER (window), canvas);
    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}

static void paint_selection (cairo_t* cr, SelectionInfo *sel_info)
{
    if (!sel_info->active)
        return;

    cairo_save (cr);
    cairo_rectangle (cr, sel_info->x, sel_info->y, sel_info->w, sel_info->h);

    cairo_set_source_rgba (cr, 0, 0, 1, 0.2);
    cairo_fill_preserve (cr);

    cairo_set_source_rgba (cr, 0, 0, 0, 0.5);
    cairo_stroke (cr);
    cairo_restore (cr);
}

static void paint (GtkWidget* widget, GdkEventExpose *eev, SelectionInfo* sel_info)
{
    gint width, height;
    gint i;
    cairo_t *cr;

    width  = widget->allocation.width;
    height = widget->allocation.height;
  
    cr = gdk_cairo_create (widget->window);
  
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);
    
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_save (cr);
    cairo_set_font_size (cr, 20);
    cairo_move_to (cr, 40, 60);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_show_text (cr, "Drag your mouse here");
    cairo_restore (cr);

    cairo_set_source_rgb (cr, 1, 0, 0);
    cairo_set_font_size (cr, 15);
    cairo_move_to (cr, 50, 100);
    cairo_show_text (cr, "(and see alpha blended selection)");

    cairo_set_source_rgb (cr, 0, 0, 1);

    cairo_move_to (cr, 0, 150);
    for (i = 0; i < width / 10; ++i) {
        cairo_rel_line_to (cr, 5,  10);
        cairo_rel_line_to (cr, 5, -10);
    }

    cairo_stroke (cr);
    paint_selection (cr, sel_info);
    cairo_destroy (cr);
}

static gboolean event_press (GtkWidget* widget, GdkEventButton* bev, SelectionInfo* sel_info)
{
    sel_info->active = TRUE;

    sel_info->x = bev->x;
    sel_info->y = bev->y;
    sel_info->w = 0;
    sel_info->h = 0;

    gtk_widget_queue_draw (widget);

    return TRUE;
}

static gboolean event_motion (GtkWidget* widget, GdkEventMotion *mev, SelectionInfo* sel_info)
{
    sel_info->w = mev->x - sel_info->x;
    sel_info->h = mev->y - sel_info->y;

    gtk_widget_queue_draw (widget);
    return TRUE;
}

static gboolean event_release (GtkWidget* widget, GdkEventButton *bev, SelectionInfo* sel_info)
{
    sel_info->active = FALSE;
    gtk_widget_queue_draw (widget);
    return TRUE;
}
