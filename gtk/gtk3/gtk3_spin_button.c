/*************************************************************************
> FileName: gtk3_spin_button.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月24日 星期三 11时04分11秒
 ************************************************************************/
#include <gtk/gtk.h>

int main (int argc, char** argv)
{
    gtk_init (&argc, &argv);

    GtkWidget* widget = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    GtkAdjustment* adjust = gtk_adjustment_new (100, 0, 1000, 2, 10, 10);

    GtkWidget* button = gtk_spin_button_new (adjust, 5, 2);

    gtk_container_add (widget, button);

    gtk_widget_show_all (widget);

    gtk_main ();

    return 0;
}
