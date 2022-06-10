/*************************************************************************
> FileName: gtk2_paned_h_mv.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 10时35分18秒
 ************************************************************************/

#include <gtk/gtk.h>

enum 
{
    PIXBUF_COL,
    TEXT_COL
};

GtkTreeModel* createModel() 
{
    gint i;
    GtkTreeIter iter;
    GdkPixbuf *pixbuf;
    GtkListStore *store;
    const gchar *files[] = {"resource/1.png", "resource/2.png", "resource/3.png", "resource/4.png"};

    store = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);

    for(i = 0; i < 4; i++) {
        pixbuf = gdk_pixbuf_new_from_file(files[i], NULL);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, PIXBUF_COL, pixbuf, TEXT_COL, files[i], -1);       
        gdk_pixbuf_unref(pixbuf);
    }

    return GTK_TREE_MODEL(store);
}

GtkWidget* createTreeView() 
{
    GtkWidget *treeView;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    treeView = gtk_tree_view_new_with_model(createModel());

    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes("Icon", renderer, "pixbuf", PIXBUF_COL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Filename", renderer, "text", TEXT_COL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (treeView), column);

    return treeView;
}

int main(int argc, char *argv[]) 
{
    GtkWidget *label;
    GtkWidget *window;
    GtkWidget *treeView;
    GtkWidget *hPaned, *vPaned;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkPaned");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);

    hPaned = gtk_hpaned_new();
    vPaned = gtk_vpaned_new();

    treeView = createTreeView();

    gtk_paned_add2(GTK_PANED(hPaned), vPaned);
    gtk_paned_add1(GTK_PANED(hPaned), treeView);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span foreground='blue' size='x-large'>Hello!World!</span>");

    gtk_paned_add1(GTK_PANED(vPaned), label);
    gtk_paned_add2(GTK_PANED(vPaned), gtk_text_view_new());

    gtk_container_add(GTK_CONTAINER(window), hPaned);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
