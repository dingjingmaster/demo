/*************************************************************************
> FileName: gtk2_combox.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月07日 星期五 20时02分01秒
 ************************************************************************/

#include <gtk/gtk.h>

gboolean combo_changed(GtkComboBox *comboBox, GtkLabel *label)
{
    gchar *active = gtk_combo_box_get_active_text(comboBox);
    gtk_label_set_text(label, active);
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *comboBox;
    GtkWidget *label;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkComboBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 50);

    comboBox = gtk_combo_box_new_text();
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboBox), "caterpillar");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboBox), "momor");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboBox), "hamimi");
    gtk_combo_box_append_text(GTK_COMBO_BOX(comboBox), "bush");
    gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox), 0);

    label = gtk_label_new("caterpillar");
    vbox = gtk_vbox_new(TRUE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), comboBox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(comboBox), "changed", G_CALLBACK(combo_changed), label);
    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
