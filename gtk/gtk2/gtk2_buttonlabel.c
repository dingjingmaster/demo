/*************************************************************************
> FileName: buttonlabel.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 10时01分53秒
 ************************************************************************/
#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer gdata)
{
    g_print ("Quitting...\n");
    gtk_main_quit ();

    return (FALSE);
}

void LabelModify (GtkWidget *child, gpointer data)
{
    char      *szText;
    GList     *children;

    if (GTK_IS_LABEL (child)) {
        gtk_label_get (GTK_LABEL (child), &szText);
        if (!strcmp (szText, "Windows")) {
            gtk_label_set (GTK_LABEL (child), "Linux");
        } else if (!strcmp (szText, "Linux")) {
            gtk_label_set (GTK_LABEL (child), "AmigaOS");
        } else {
            gtk_label_set (GTK_LABEL (child), "Windows");
        }
    }
}

void button_was_clicked (GtkWidget *widget, gpointer gdata)
{
    gtk_container_foreach (GTK_CONTAINER (widget), (GtkCallback) LabelModify, NULL);
}

int main (int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_border_width (GTK_CONTAINER (window), 15);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", 
                GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    button = gtk_button_new_with_label ("Click me!");

    gtk_signal_connect (GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (button_was_clicked), "clicked");

    gtk_container_add (GTK_CONTAINER (window), button);

    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Windows"); 

    gtk_widget_show (button);
    gtk_widget_show (window);

    gtk_main ();

    return 0;
}

