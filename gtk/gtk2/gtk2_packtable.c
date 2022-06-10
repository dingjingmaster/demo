/*************************************************************************
> FileName: packtable.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 09时00分27秒
 ************************************************************************/
#include <gtk/gtk.h>

void Destroy (GtkWidget *widget, gpointer *data)
{
    printf ("destroy\n");
}

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    printf ("delete\n");

    gtk_main_quit ();
    return (FALSE);
}
    
CreateTable (char *szTitle, gint xoptions, gint yoptions) 
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *table;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), szTitle);
    gtk_container_border_width (GTK_CONTAINER (window), 10);

    gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (Destroy), NULL);
    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    table = gtk_table_new (4, 4, TRUE);

    /* --- Add a 1x1 button at 0, 0 --- */
    button = gtk_button_new_with_label ("Button at 0,0");
    gtk_table_attach (GTK_TABLE (table), button, 0, 1, 0, 1, xoptions, yoptions, 0, 0);
    gtk_widget_show (button);

    /* --- Add a 2x2 button at 2-4, 2-4 --- */
    button = gtk_button_new_with_label ("Big square button");
    gtk_table_attach (GTK_TABLE (table), button, 2, 4, 2, 4, xoptions, yoptions, 0, 0);
    gtk_widget_show (button);

    /* --- Add a 2x1 short wide button --- */
    button = gtk_button_new_with_label ("Short fat button");
    gtk_table_attach (GTK_TABLE (table), button, 0, 2, 3, 4, xoptions, yoptions, 0, 0);
    gtk_widget_show (button);

    /* --- Add a 1x2 tall skinny button --- */
    button = gtk_button_new_with_label ("Tall skinny button");
    gtk_table_attach (GTK_TABLE (table), button, 3, 4, 0, 2, xoptions, yoptions, 0, 0);

    gtk_widget_show (button);

    gtk_container_add (GTK_CONTAINER (window), table);
    gtk_widget_show (table);
    gtk_widget_show (window);
}

int main (int argc, char *argv[])
{
    
    gtk_init (&argc, &argv);

    // 固定地点的图标，不会随着父窗口改变而改变
    CreateTable ("Nothing", 0, 0);

    // 同上
    CreateTable ("Fill", GTK_FILL, GTK_FILL);

    // 同上
    CreateTable ("Shrink", GTK_SHRINK, GTK_SHRINK);

    // 子部件位置随着父窗口大小的改变而改变，相对位置不会变
    CreateTable ("Expand", GTK_EXPAND, GTK_EXPAND);

    CreateTable ("Expand | Shrink", GTK_EXPAND | GTK_SHRINK, GTK_EXPAND | GTK_SHRINK);

    CreateTable ("Fill | Expand", GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL);

    CreateTable ("Shrink | Fill", GTK_SHRINK | GTK_FILL, GTK_SHRINK | GTK_FILL);

    CreateTable ("Shrink | Fill | Expand", GTK_EXPAND | GTK_SHRINK | GTK_FILL, GTK_EXPAND | GTK_SHRINK | GTK_FILL);

    gtk_main ();

    exit (0);
}
