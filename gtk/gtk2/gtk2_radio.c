/*************************************************************************
> FileName: radio.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 09时36分52秒
 ************************************************************************/
#include <gtk/gtk.h>

void Destroy (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
}

void RadioEvent (GtkWidget *widget, gpointer *data)
{
    g_print ("radio event: %s\n", data);
}

GtkWidget *CreateRadio (GtkWidget *box, GSList **group, char *szLabel)
{
    GtkWidget *radio;

    radio = gtk_radio_button_new_with_label (*group, szLabel);

    *group = gtk_radio_button_group (GTK_RADIO_BUTTON (radio));
    gtk_box_pack_start (GTK_BOX (box), radio, FALSE, FALSE, 10);
    gtk_widget_show (radio);

    return (radio);
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *radio;
    GSList *group = NULL;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Radio buttons");
    gtk_container_border_width (GTK_CONTAINER (window), 50);

    gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (Destroy), NULL);

    vbox = gtk_vbox_new (FALSE, 0);
    radio = CreateRadio (vbox, &group, "Have Linux");

    gtk_signal_connect (GTK_OBJECT (radio), "toggled", GTK_SIGNAL_FUNC (RadioEvent), "radio 1 toggled");
    gtk_signal_connect (GTK_OBJECT (radio), "pressed", GTK_SIGNAL_FUNC (RadioEvent), "radio 1 pressed");
    gtk_signal_connect (GTK_OBJECT (radio), "released", GTK_SIGNAL_FUNC (RadioEvent), "radio 1 released");
    gtk_signal_connect (GTK_OBJECT (radio), "clicked", GTK_SIGNAL_FUNC (RadioEvent), "radio 1 clicked");
    gtk_signal_connect (GTK_OBJECT (radio), "enter", GTK_SIGNAL_FUNC (RadioEvent), "radio 1 enter");
    gtk_signal_connect (GTK_OBJECT (radio), "leave", GTK_SIGNAL_FUNC (RadioEvent), "radio 1 leave");

    radio = CreateRadio (vbox, &group, "Use Linux");

    gtk_signal_connect (GTK_OBJECT (radio), "toggled", GTK_SIGNAL_FUNC (RadioEvent), "radio 2 toggled");
    gtk_signal_connect (GTK_OBJECT (radio), "pressed", GTK_SIGNAL_FUNC (RadioEvent), "radio 2 pressed");
    gtk_signal_connect (GTK_OBJECT (radio), "released", GTK_SIGNAL_FUNC (RadioEvent), "radio 2 released");
    gtk_signal_connect (GTK_OBJECT (radio), "clicked", GTK_SIGNAL_FUNC (RadioEvent), "radio 2 clicked");
    gtk_signal_connect (GTK_OBJECT (radio), "enter", GTK_SIGNAL_FUNC (RadioEvent), "radio 2 enter");
    gtk_signal_connect (GTK_OBJECT (radio), "leave", GTK_SIGNAL_FUNC (RadioEvent), "radio 2 leave");

    gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (radio), TRUE);

    radio = CreateRadio (vbox, &group, "Like Linux");

    gtk_signal_connect (GTK_OBJECT (radio), "toggled", GTK_SIGNAL_FUNC (RadioEvent), "radio 3 toggled");
    gtk_signal_connect (GTK_OBJECT (radio), "pressed", GTK_SIGNAL_FUNC (RadioEvent), "radio 3 pressed");
    gtk_signal_connect (GTK_OBJECT (radio), "released", GTK_SIGNAL_FUNC (RadioEvent), "radio 3 released");
    gtk_signal_connect (GTK_OBJECT (radio), "clicked", GTK_SIGNAL_FUNC (RadioEvent), "radio 3 clicked");
    gtk_signal_connect (GTK_OBJECT (radio), "enter", GTK_SIGNAL_FUNC (RadioEvent), "radio 3 enter");
    gtk_signal_connect (GTK_OBJECT (radio), "leave", GTK_SIGNAL_FUNC (RadioEvent), "radio 3 leave");

    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);
}

