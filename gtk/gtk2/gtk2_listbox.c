/*************************************************************************
> FileName: listbox.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月13日 星期五 09时43分20秒
 ************************************************************************/
#include <gtk/gtk.h>

void AddListItem (GtkWidget *listbox, char *sText);

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return FALSE;
}

void listitem_selected (GtkWidget *widget, gpointer *data)
{
    g_print ("item selected - %s\n", data);
}

char *GetItemLabel (GtkWidget *widget)
{
    GtkWidget *label = NULL;
    char *str = NULL;
    GList *children;
    GtkWidget *child;
    
    /* --- Get list of children in the list item --- */
    children = gtk_container_children (GTK_CONTAINER (widget));

    /* --- Get the widget --- */
    child = GTK_WIDGET (children->data);

    /* --- If the widget is a label --- */
    if (GTK_IS_LABEL (child)) {
        /* --- Get the string in the label --- */
        gtk_label_get (GTK_LABEL (child), &str);
    }

    return (str);
}

void DisplaySelectedItems (GtkWidget *widget)
{
    GList *children;
    GtkWidget *child;
    char *str = NULL;

    /////////////////////////////////////////////////////////
    // 无法打印选中控件的 label
    if (GTK_IS_LABEL (widget)) {
        gtk_label_get (GTK_LABEL (child), &str);
    }
    printf ("=============================\n");
    printf ("--> %s\n", str);
    printf ("=============================\n");
    /////////////////////////////////////////////////////////

    /* --- Get children in the listbox (list items) --- */
    // 必须先获取父控件，根据父控件获取子控件，然后再遍历？？
    // 事件触发传入的 widget 到底是哪个widget
    children = gtk_container_children (GTK_CONTAINER (widget));

    while (children) {
        /* --- Get child's widget information --- */
        child = GTK_WIDGET (children->data);

        /* --- Display list item information --- */
        printf ("state = %d, label = %s\n", 
                (int) GTK_WIDGET_STATE (child), 
                GetItemLabel (child));

        /* --- Go to the next child --- */
        children = children->next;
    }
}

void listbox_changed (GtkWidget *widget, gpointer *data)
{
    GtkList *listbox;

    g_print ("%s\n", data);
    DisplaySelectedItems (widget);
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *listbox;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_border_width (GTK_CONTAINER (window), 10);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
            GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    listbox = gtk_list_new ();

    gtk_signal_connect (GTK_OBJECT (listbox), "selection_changed",
            GTK_SIGNAL_FUNC (listbox_changed), "selection_changed");

    /* --- Set listbox style. --- */
    gtk_list_set_selection_mode (GTK_LIST (listbox), GTK_SELECTION_BROWSE);

    gtk_widget_show (listbox);

    AddListItem (listbox, "This is a listbox");
    AddListItem (listbox, "Quite useful ... ");
    AddListItem (listbox, "When it needs to be.");
    AddListItem (listbox, "This list can be ");
    AddListItem (listbox, "quite long, you know.");

    gtk_container_add (GTK_CONTAINER (window), listbox);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}

/*
 * AddListItem
 *
 * Add the text to the listbox.
 */
void AddListItem (GtkWidget *listbox, char *sText)
{
    GtkWidget     *item;

    /* --- Create a list item from the data element --- */
    item = gtk_list_item_new_with_label (sText);

    /* --- Conigure for the "select" event --- */
    gtk_signal_connect (GTK_OBJECT (item), "select",
            GTK_SIGNAL_FUNC (listitem_selected), sText);

    /* --- Add item to the listbox --- */
    gtk_container_add (GTK_CONTAINER (listbox), item);

    /* --- Make it visible --- */
    gtk_widget_show (item);
}

