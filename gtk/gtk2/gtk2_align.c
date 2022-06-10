/*************************************************************************
> FileName: align.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 10时05分30秒
 ************************************************************************/
#include <gtk/gtk.h>

void AddListItem (GtkWidget *listbox, char *sText);

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
    return (FALSE);
}


int main (int argc, char *argv[])
{
    GList *cbitems = NULL;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *vbox;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_border_width (GTK_CONTAINER (window), 10);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    vbox = gtk_hbox_new (FALSE, 0);

    label = gtk_label_new ("label");
	
    /* --- Align the label to the left side.   --- */
    // x=0, y=0 总长度
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);

    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    label = gtk_label_new ("center");
	
    /* --- Align the label to the right side.   --- */
    // x=0, y=0.5
    gtk_misc_set_alignment (GTK_MISC (label), 1, .5);

    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    label = gtk_label_new ("right");
	
    /* --- Align the label to the center.   --- */
    // y=0.5 x=1
    gtk_misc_set_alignment (GTK_MISC (label), .5, 1);

    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    button = gtk_button_new_with_label ("Button");

    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);
    gtk_widget_show (vbox);

    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (window);

    gtk_main ();
}

