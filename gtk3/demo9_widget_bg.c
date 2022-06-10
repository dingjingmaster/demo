#include <gtk/gtk.h>

int main(int argc,char *argv[]){

GtkWidget *win;

GtkWidget *image;

GtkWidget *frame;

gtk_init(&argc,&argv);

win=gtk_window_new(GTK_WINDOW_TOPLEVEL);

g_signal_connect((win),"destroy",G_CALLBACK(gtk_main_quit),NULL);

image=gtk_image_new_from_file("../warning_bg.png");

frame=gtk_frame_new(NULL);

gtk_container_add(GTK_CONTAINER(frame),image);

gtk_container_add(GTK_CONTAINER(win),frame);

gtk_widget_show_all(win);

gtk_main();

return 0;

}
