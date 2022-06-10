/*************************************************************************
> FileName: toggle.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 09时41分32秒
 ************************************************************************/
#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}

void ButtonEvent (GtkWidget *widget, gpointer *data)
{
    g_print ("Button event: %s\n", data);
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *vbox;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_border_width (GTK_CONTAINER (window), 50);
    gtk_window_set_title (GTK_WINDOW (window), "Toggle Button");

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    vbox = gtk_vbox_new (FALSE, 0);

    button = gtk_toggle_button_new_with_label ("Top Toggle Button");
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);

    gtk_signal_connect (GTK_OBJECT (button), "toggled",
			GTK_SIGNAL_FUNC (ButtonEvent), "top toggled");
    gtk_signal_connect (GTK_OBJECT (button), "pressed",
			GTK_SIGNAL_FUNC (ButtonEvent), "top pressed");
    gtk_signal_connect (GTK_OBJECT (button), "released",
			GTK_SIGNAL_FUNC (ButtonEvent), "top released");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ButtonEvent), "top clicked");
    gtk_signal_connect (GTK_OBJECT (button), "enter",
			GTK_SIGNAL_FUNC (ButtonEvent), "top enter");
    gtk_signal_connect (GTK_OBJECT (button), "leave",
			GTK_SIGNAL_FUNC (ButtonEvent), "top leave");

    button = gtk_toggle_button_new_with_label ("Bottom Toggle Button");
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);

    gtk_signal_connect (GTK_OBJECT (button), "toggled",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom toggled");
    gtk_signal_connect (GTK_OBJECT (button), "pressed",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom pressed");
    gtk_signal_connect (GTK_OBJECT (button), "released",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom released");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom clicked");
    gtk_signal_connect (GTK_OBJECT (button), "enter",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom enter");
    gtk_signal_connect (GTK_OBJECT (button), "leave",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom leave");

    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}
