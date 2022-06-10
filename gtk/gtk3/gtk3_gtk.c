/*************************************************************************
> FileName: gtk.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月30日 星期一 17时02分53秒
 ************************************************************************/
#include <gtk/gtk.h>

int main (int argc, char* argv)
{
    GtkWidget* widget = NULL;

    widget = gtk_widget_new (GTK_TYPE_WIDGET, NULL);

    return 0;
}
