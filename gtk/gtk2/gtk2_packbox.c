/*************************************************************************
> FileName: packbox.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月12日 星期四 11时24分38秒
 ************************************************************************/
#include <gtk/gtk.h>

void AddListItem (GtkWidget *listbox, char *sText);

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
    return FALSE;
}

int main (int argc, char *argv[])
{
    GList *cbitems = NULL;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *entry;
    GtkWidget *combo;
    GtkWidget *vbox;
    GtkWidget *box2;
    GtkWidget *check;
    GtkWidget *label;
    GtkWidget *listbox;
    int which;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* 边框 */
    gtk_container_border_width (GTK_CONTAINER (window), 10);

    vbox = gtk_vbox_new (FALSE, 0);
	
    /* label 左对齐 */
    label = gtk_label_new ("label");
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    // 按钮
    button = gtk_button_new_with_label ("Button");
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);
	
    // 输入
    entry = gtk_entry_new_with_max_length (20);
    gtk_box_pack_start (GTK_BOX (vbox), entry, FALSE, FALSE, 0);
    gtk_entry_set_text (GTK_ENTRY (entry), "entry field");
    gtk_widget_show (entry);

    // 复选框
    check = gtk_check_button_new_with_label ("Check button");
    gtk_box_pack_start (GTK_BOX (vbox), check, FALSE, FALSE, 0);
    gtk_widget_show (check);

    // list列表
    listbox = gtk_list_new ();
    gtk_list_set_selection_mode (GTK_LIST (listbox), GTK_SELECTION_BROWSE);
    gtk_box_pack_start (GTK_BOX (vbox), listbox, FALSE, FALSE, 0);
    gtk_widget_show (listbox);

    AddListItem (listbox, "This is a listbox");
    AddListItem (listbox, "Quite useful ... ");
    AddListItem (listbox, "When it needs to be.");
    AddListItem (listbox, "This list can be ");
    AddListItem (listbox, "quite long, you know.");

    cbitems = g_list_append (cbitems, "Car");
    cbitems = g_list_append (cbitems, "House");
    cbitems = g_list_append (cbitems, "Job");
    cbitems = g_list_append (cbitems, "Computer");

    combo = gtk_combo_new ();
    gtk_combo_set_popdown_strings (GTK_COMBO(combo), cbitems);
    gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(combo)->entry), "Hello");
    gtk_box_pack_start (GTK_BOX (vbox), combo, FALSE, FALSE, 0);
    gtk_widget_show (combo);

    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);
}

void AddListItem (GtkWidget *listbox, char *sText)
{
	GtkWidget 	*item;

	item = gtk_list_item_new_with_label (sText);
	gtk_container_add (GTK_CONTAINER (listbox), item);
	gtk_widget_show (item);
}
