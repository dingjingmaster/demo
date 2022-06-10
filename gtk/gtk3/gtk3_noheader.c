/*************************************************************************
> FileName: noheader.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月10日 星期二 13时25分45秒
 ************************************************************************/
#include <gtk/gtk.h>

int main(int argc,char **argv)
{
    GtkWidget *win;
    GtkWidget *button;
    /* 这个函数在所有的 GTK 程序都要调用。参数由命令行中解析出来并且送到该程序中*/
    gtk_init(&argc,&argv);

    win=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(win,500,400);

    gtk_window_set_title(GTK_WINDOW(win),"FileDialog");
    gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);

    gtk_window_set_decorated (GTK_WINDOW(win),FALSE);	//这一行去边框
    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
