/*************************************************************************
> FileName: gtk2_combo_list_store.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月07日 星期五 21时02分13秒
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
    GdkPixbuf *pixbuf;
    GtkTreeIter iter;
    GtkListStore *store;
    gint i;

    store = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);

    for(i = 0; i < 4; i++) {
        pixbuf = gdk_pixbuf_new_from_file(files[i], NULL);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, PIXBUF_COL, pixbuf, TEXT_COL, files[i], -1);       
        gdk_pixbuf_unref(pixbuf);
    }

    return GTK_TREE_MODEL(store);
}

gboolean combo_changed(GtkComboBox *comboBox, GtkLabel *label) 
{
    GtkTreeModel *model = gtk_combo_box_get_model(comboBox);
    GtkTreeIter iter;
    gchar *active;

    gtk_combo_box_get_active_iter(comboBox, &iter);
    gtk_tree_model_get(model, &iter, 1, &active, -1);   // 参数：model、激活的那一列、第几列、激活那一列的值、结束符号

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


    // ComboBox 模型部分
    comboBox = gtk_combo_box_new_with_model(createModel());
    gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox), 0);

    renderer = gtk_cell_renderer_pixbuf_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox), renderer, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox), renderer, "pixbuf", PIXBUF_COL,  NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox), renderer, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox), renderer, "text", TEXT_COL, NULL);    


    // 主界面布局
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
