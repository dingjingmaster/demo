/*************************************************************************
> FileName: gtk2_combo_tree_store.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 09时18分13秒
 ************************************************************************/

#include <gtk/gtk.h>

enum 
{
    PIXBUF_COL,
    TEXT_COL
};

GtkTreeModel* createModel() 
{
    const gchar *files[] = {"resource/1.png", "resource/2.png", "resource/3.png", "resource/4.png"};
    gchar *stocks[] = {
        GTK_STOCK_DIALOG_WARNING,
        GTK_STOCK_STOP,
        GTK_STOCK_NEW,
        GTK_STOCK_CLEAR,
        GTK_STOCK_OPEN    
    };

    gchar *stockNames[] = {
        "WARNING",
        "STOP",
        "NEW",
        "GTK_STOCK_CLEAR",
        "GTK_STOCK_OPEN"
    };

    gint i, j, s;
    GdkPixbuf *pixbuf;
    GtkWidget *cellView;
    GtkTreeStore *store;
    GtkTreeIter iter1, iter2;

    store = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
    cellView = gtk_cell_view_new();
    for(i = 0; i < 4; i++) {
        pixbuf = gdk_pixbuf_new_from_file(files[i], NULL);
        gtk_tree_store_append(store, &iter1, NULL);
        gtk_tree_store_set(store, &iter1, PIXBUF_COL, pixbuf, TEXT_COL, files[i], -1);
        gdk_pixbuf_unref(pixbuf);

        for(j = 0; j < 3; j++) {
            s = rand() % 5;
            pixbuf = gtk_widget_render_icon(cellView, stocks[s], GTK_ICON_SIZE_BUTTON, NULL);
            // 第一个是追加的节点，第二个是子节点
            gtk_tree_store_append(store, &iter2, &iter1);
            gtk_tree_store_set(store, &iter2, PIXBUF_COL, pixbuf, TEXT_COL, stockNames[s], -1);
            gdk_pixbuf_unref(pixbuf);
        }
    }

    return GTK_TREE_MODEL(store);
}

gboolean combo_changed(GtkComboBox *comboBox, GtkLabel *label) 
{
    gchar *active;
    GtkTreeIter iter;
    GtkTreeModel *model = gtk_combo_box_get_model(comboBox);

    // 激活的那个子节点
    gtk_combo_box_get_active_iter(comboBox, &iter);
    gtk_tree_model_get(model, &iter, 1, &active, -1);

    gtk_label_set_text(label, active);
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *comboBox;
    GtkCellRenderer *renderer;
    GtkWidget *label;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkComboBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 50);

    comboBox = gtk_combo_box_new_with_model(createModel());
    gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox), 0);

    // 第一列
    renderer = gtk_cell_renderer_pixbuf_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox), renderer, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox), renderer, "pixbuf", PIXBUF_COL,  NULL);

    // 第二列
    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox), renderer, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox), renderer, "text", TEXT_COL, NULL);    

    label = gtk_label_new("resource/1.png");
    vbox = gtk_vbox_new(TRUE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), comboBox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(GTK_OBJECT(comboBox), "changed", G_CALLBACK(combo_changed), label);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
