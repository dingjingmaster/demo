/*************************************************************************
> FileName: gtkoverlay.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月10日 星期二 10时09分19秒
 ************************************************************************/

#include <gtk/gtk.h>  
void gtk_overlay_set_overlay_pass_through (GtkOverlay *overlay, GtkWidget *widget, gboolean pass_through)
{
    g_return_if_fail (GTK_IS_OVERLAY (overlay));
	g_return_if_fail (GTK_IS_WIDGET (widget));
	gtk_container_child_set (GTK_CONTAINER (overlay), widget,
	"pass-through", pass_through,
	NULL);
}
	
static void activate (GtkApplication *app , gpointer data)  
{  
    	GtkWidget *win;  
	
	GtkWidget *overlay; 
	win = gtk_application_window_new(app);  
	
	gtk_window_set_title(GTK_WINDOW(win) , " My test");  
	gtk_window_set_default_size(GTK_WINDOW(win) , 500 , 510);  
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);  
 
	overlay = gtk_overlay_new ();
	GtkWidget *image = NULL;
	image = gtk_image_new_from_file ("pic/2.svg");
	
	GtkWidget *image_background = NULL;
	image_background = gtk_image_new_from_file ("pic/1.svg");
    gtk_container_add(GTK_CONTAINER(overlay), image_background);
	gtk_overlay_add_overlay (GTK_OVERLAY (overlay), image);
	
	//gtk_overlay_set_overlay_pass_through(GTK_OVERLAY (overlay), label, TRUE);
 
	gtk_container_add (GTK_CONTAINER (win), overlay);
	
 
	gtk_widget_show_all(win);  
}  
  
int main(int argc , char **argv)  
{  
	GtkApplication *app;  
	int app_status;  
	app = gtk_application_new("org.gtk.exmple" , G_APPLICATION_FLAGS_NONE);  
	g_signal_connect(app , "activate" , G_CALLBACK(activate) , NULL);  
	app_status = g_application_run(G_APPLICATION(app) , argc , argv);  
	g_object_unref(app);  	
	return app_status;  
}  
