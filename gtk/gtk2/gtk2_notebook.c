/*************************************************************************
> FileName: gtk2_notebook.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 10时24分36秒
 ************************************************************************/

#include <gtk/gtk.h>

GtkWidget* createTab(gchar *filename, gchar *text) 
{
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *image;

    box = gtk_hbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(box), 2);

    image = gtk_image_new_from_file(filename);
    label = gtk_label_new(text);

    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);

    gtk_widget_show (image);
    gtk_widget_show (label);

    return box;    
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *tab;
    GtkWidget *label;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkNotebook");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    notebook = gtk_notebook_new();

    tab = createTab("resource/1.png", "caterpillar");
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span foreground='blue' size='x-large'>Hello!World!</span>");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, tab);

    tab = createTab("resource/2.png", "momor");
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<big>Big text 2</big>");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, tab);

    tab = createTab("resource/3.png", "hamimi");
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<big>Big text 3</big>");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), label, tab);

    gtk_container_add(GTK_CONTAINER(window), notebook);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
