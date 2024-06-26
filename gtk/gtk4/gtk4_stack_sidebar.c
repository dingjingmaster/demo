/*************************************************************************
> FileName: gtk4/gtk4_stack_sidebar.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年09月08日 星期三 15时53分21秒
 ************************************************************************/

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget* do_sidebar (GtkWidget *do_widget)
{
  static GtkWidget *window = NULL;
  GtkWidget *sidebar;
  GtkWidget *stack;
  GtkWidget *box;
  GtkWidget *widget;
  GtkWidget *header;
  const char * pages[] = {
    "Welcome to GTK",
    "GtkStackSidebar Widget",
    "Automatic navigation",
    "Consistent appearance",
    "Scrolling",
    "Page 6",
    "Page 7",
    "Page 8",
    "Page 9",
    NULL
  };
  const char *c = NULL;
  guint i;

  if (!window)
    {
      window = gtk_window_new ();
      gtk_window_set_resizable (GTK_WINDOW (window), TRUE);

      header = gtk_header_bar_new ();
      gtk_window_set_titlebar (GTK_WINDOW(window), header);
      gtk_window_set_title (GTK_WINDOW(window), "Stack Sidebar");
      g_object_add_weak_pointer (G_OBJECT (window), (gpointer *)&window);

      box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
      sidebar = gtk_stack_sidebar_new ();
      gtk_box_append (GTK_BOX (box), sidebar);

      stack = gtk_stack_new ();
      gtk_stack_set_transition_type (GTK_STACK (stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
      gtk_stack_sidebar_set_stack (GTK_STACK_SIDEBAR (sidebar), GTK_STACK (stack));
      gtk_widget_set_hexpand (stack, TRUE);

      gtk_box_append (GTK_BOX (box), stack);

      for (i=0; (c = *(pages+i)) != NULL; i++ )
        {
          if (i == 0)
            {
              widget = gtk_image_new_from_icon_name ("org.gtk.Demo4");
              gtk_widget_add_css_class (widget, "icon-dropshadow");
              gtk_image_set_pixel_size (GTK_IMAGE (widget), 256);
            }
          else
            {
              widget = gtk_label_new (c);
            }
          gtk_stack_add_named (GTK_STACK (stack), widget, c);
          g_object_set (gtk_stack_get_page (GTK_STACK (stack), widget), "title", c, NULL);
        }

      gtk_window_set_child (GTK_WINDOW (window), box);
    }

  if (!gtk_widget_get_visible (window))
    gtk_widget_show (window);
  else
    gtk_window_destroy (GTK_WINDOW (window));

  return window;
}

static void activate (GApplication* app)
{
    GtkWidget* w = gtk_application_window_new (app);
    GtkWidget* wi = do_sidebar(w);
    gtk_widget_show (wi);
}

int main (int argc, char* argv[])
{
    GtkApplication *app = gtk_application_new ("com.GtkApplication", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    return g_application_run (G_APPLICATION (app), argc, argv);
}

