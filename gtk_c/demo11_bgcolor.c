#include <gtk/gtk.h>

int main(int argc, char *argv[] )
{
  GtkWidget *window;
  GtkWidget *button;
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
  GtkStyleContext *context;
  
  gtk_init (&argc, &argv);
  
 window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 280, 200);
  gtk_window_set_title(GTK_WINDOW(window),"CSSBUTTON");
  g_signal_connect (GTK_WIDGET (window), "destroy",
  G_CALLBACK (gtk_main_quit), NULL);
  
 button = gtk_button_new_with_label("CssButton");  
  gtk_widget_set_name (GTK_WIDGET(button), "mybutton");        
  gtk_widget_set_halign (GTK_WIDGET(button),GTK_ALIGN_CENTER);
  gtk_widget_set_valign (GTK_WIDGET(button),GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(GTK_WIDGET(button), 200, 120);
  gtk_container_add(GTK_CONTAINER(window),button); 
  
  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);                                                                                  
  context = gtk_widget_get_style_context(window);
  
  gtk_css_provider_load_from_data (provider,
  "*{"
  "color:green;"
  "font-family:Monospace;"
  "border:1px solid;"
  " }"       
  " window {"                          
  "   background-color: rgba(50,30,70,100);"
  "   background-image:none;"
  "}"
  " button {"
  "   border-radius: 15px;"
  "   font: Sans 23;"
  "   color: #00008B;"                                       
  "   background-color: green;"
  "   background-image:none;"
  "}"
  " button:hover {"
  "   background-color: red;"
  "   color: white;"
  "}"
  " button:hover:active {"
  "   background-color: orange;"
  "   color: cyan;"
  "}", -1, NULL);
  
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref (provider);
  gtk_widget_show_all(window);
  gtk_main();
  return(0);
}
