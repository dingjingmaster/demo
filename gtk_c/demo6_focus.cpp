#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

void hello_proc(GtkWidget* w,gpointer userdata)
{
//    g_printf("hello button %d",(int)userdata);
}

#define WIDTH 480
#define HEIGHT 320

int main(int argc,char** argv)
{
    gtk_init(&argc,&argv);
    GtkWidget* main_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(main_win,WIDTH,HEIGHT);
    gtk_container_set_border_width(GTK_CONTAINER(main_win),5);

    //创建垂直布局
    GtkWidget* v = gtk_vbox_new(FALSE,5);
    gtk_container_add(GTK_CONTAINER(main_win),v);

    //创建按钮
    GtkWidget* b1 = gtk_button_new_with_label("GTK+ Button1");
    gtk_box_pack_start(GTK_BOX(v),b1,FALSE,FALSE,0);
    //设置按钮的单击事件回调函数，参数为1
    g_signal_connect(
        G_OBJECT(b1),"clicked",
        G_CALLBACK(hello_proc),GSIZE_TO_POINTER(1));

    //创建按钮
    GtkWidget* b2 = gtk_button_new_with_label("GTK+ Button2");
    gtk_box_pack_start(GTK_BOX(v),b2,FALSE,FALSE,0);
    //设置按钮的单击事件回调函数，参数为2
    g_signal_connect(
        G_OBJECT(b2),"clicked",
        G_CALLBACK(hello_proc),GSIZE_TO_POINTER(2));

    //创建按钮
    GtkWidget* b3 = gtk_button_new_with_label("GTK+ Button3");
    gtk_box_pack_start(GTK_BOX(v),b3,FALSE,FALSE,0);
    //设置按钮的单击事件处理函数，参数为3
    g_signal_connect(
        G_OBJECT(b3),"clicked",
        G_CALLBACK(hello_proc),GSIZE_TO_POINTER(3));

    //设置主窗口的焦点在第二个按钮上
    gtk_window_set_focus(GTK_WINDOW(main_win),b2);


    g_signal_connect_swapped(
        G_OBJECT(main_win),"destroy",
        G_CALLBACK(gtk_main_quit),NULL);

    //显示所有窗口
    gtk_widget_show_all(main_win);
    gtk_main();

    return 0;
}
