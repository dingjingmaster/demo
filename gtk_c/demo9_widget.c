#include <cairo.h>
#include <gtk/gtk.h>


gboolean timer = TRUE;
cairo_surface_t *image;


static gboolean
on_expose_event(GtkWidget *widget,
    GdkEventExpose *event,
    gpointer data)
{
  cairo_t *cr;
  cairo_t *ic;

  cairo_surface_t *surface;

  static gdouble angle = 0;
  static gint image_width = 0;
  static gint image_height = 0;

  static gint w = 0;
  static gint h = 0;

  cr = gdk_cairo_create(gtk_widget_get_window(widget));

  gint width, height;
  gtk_window_get_size(GTK_WINDOW(widget), &width, &height);

  image_width = cairo_image_surface_get_width(image);
  image_height = cairo_image_surface_get_height(image);
  w = image_width;

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, image_width, image_height);
  ic = cairo_create(surface);

  cairo_rectangle(ic, 0, 0, w, h);
  cairo_fill(ic);

  h += 1;
  if ( h == image_height) timer = FALSE;

  cairo_set_source_surface(cr, image, 10, 10);
  cairo_mask_surface(cr, surface, 10, 10);

  cairo_surface_destroy(surface);

  cairo_destroy(cr);
  cairo_destroy(ic);
  return FALSE;
}

static gboolean
time_handler(GtkWidget *widget)
{
  if (gtk_widget_get_window(widget) == NULL) return FALSE;

  if (!timer) return FALSE;

  gtk_widget_queue_draw(widget);
  return TRUE;
}

int main(int argc, char *argv[])
{
  GtkWidget *window;

  image = cairo_image_surface_create_from_png("plaveckycastle.png");

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(G_OBJECT(window), "expose-event",
      G_CALLBACK(on_expose_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 325, 250);
  gtk_window_set_title(GTK_WINDOW(window), "blind down");

  gtk_widget_set_app_paintable(window, TRUE);
  g_timeout_add(15, (GSourceFunc) time_handler, (gpointer) window);

  gtk_widget_show_all(window);

  gtk_main();

  cairo_surface_destroy(image);

  return 0;
}
