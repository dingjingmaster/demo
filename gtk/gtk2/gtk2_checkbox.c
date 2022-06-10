/*************************************************************************
> FileName: checkbox.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 09时58分36秒
 ************************************************************************/
#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}

void CheckEvent (GtkWidget *widget, gpointer *data)
{
    g_print ("checkbox event: %s\n", data);
}

GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel)
{
    GtkWidget *check;

    check = gtk_check_button_new_with_label (szLabel);
    gtk_box_pack_start (GTK_BOX (box), check, FALSE, FALSE, 10);

    gtk_widget_show (check);

    return (check);
}


int main (int argc, char *argv[])
{
    GList *cbitems = NULL;
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *check;
    
    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_border_width (GTK_CONTAINER (window), 50);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    vbox = gtk_vbox_new (FALSE, 0);

    check = CreateCheckBox (vbox, "Have Linux");
    gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (check), TRUE);
    gtk_signal_connect (GTK_OBJECT (check), "toggled",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 toggled");
    gtk_signal_connect (GTK_OBJECT (check), "pressed",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 pressed");
    gtk_signal_connect (GTK_OBJECT (check), "released",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 released");
    gtk_signal_connect (GTK_OBJECT (check), "clicked",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 clicked");
    gtk_signal_connect (GTK_OBJECT (check), "enter",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 enter");
    gtk_signal_connect (GTK_OBJECT (check), "leave",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 leave");

    check = CreateCheckBox (vbox, "Use Linux");
    gtk_signal_connect (GTK_OBJECT (check), "toggled",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 toggled");
    gtk_signal_connect (GTK_OBJECT (check), "pressed",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 pressed");
    gtk_signal_connect (GTK_OBJECT (check), "released",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 released");
    gtk_signal_connect (GTK_OBJECT (check), "clicked",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 clicked");
    gtk_signal_connect (GTK_OBJECT (check), "enter",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 enter");
    gtk_signal_connect (GTK_OBJECT (check), "leave",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 leave");

    check = CreateCheckBox (vbox, "Like Linux");
    gtk_signal_connect (GTK_OBJECT (check), "toggled",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 toggled");
    gtk_signal_connect (GTK_OBJECT (check), "pressed",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 pressed");
    gtk_signal_connect (GTK_OBJECT (check), "released",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 released");
    gtk_signal_connect (GTK_OBJECT (check), "clicked",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 clicked");
    gtk_signal_connect (GTK_OBJECT (check), "enter",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 enter");
    gtk_signal_connect (GTK_OBJECT (check), "leave",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 leave");

    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}
