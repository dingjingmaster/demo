/*************************************************************************
> FileName: demo14_fix.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年11月26日 星期二 08时53分14秒
 ************************************************************************/

#include <stdio.h>

#include <gtk/gtk.h>

int main(int argc,char *argv[])
{
        //1.gtk环境初始化
        gtk_init(&argc, &argv);

        //2.创建一个窗口
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        //3.创建一个固定布局容器fixed
        GtkWidget *fixed = gtk_fixed_new();

        //将fixed添加到window中
        gtk_container_add(GTK_CONTAINER(window), fixed);

        //4.创建一个button按钮
        GtkWidget *button1 = gtk_button_new_with_label("button1");

        //将button1添加到固定容器fixed中
        gtk_fixed_put(GTK_FIXED(fixed), button1, 0,0);

        GtkWidget *button2 = gtk_button_new_with_label("button2");

        //将button1添加到固定容器fixed中
        gtk_fixed_put(GTK_FIXED(fixed), button2, 0, 0);

        //5.移动button2到(150,150)的位置
        gtk_fixed_move(GTK_FIXED(fixed), button2, 150, 150);

        //6.设置button2的大小
        gtk_widget_set_size_request(button2,100, 50);

        //7.显示所有窗口
        gtk_widget_show_all(window);

        //8.主事件循环
        gtk_main();


        return 0;
}

