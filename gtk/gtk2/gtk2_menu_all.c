/*************************************************************************
> FileName: gtk2_menu_all.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月08日 星期六 11时41分40秒
 ************************************************************************/

#include <gtk/gtk.h>

void itemPressed(GtkMenuItem *menuItem, gpointer data) 
{
    g_print("%s\n", data);
}

GtkWidget* createFileMenuItem() 
{
    GtkWidget *rootFileItem;
    GtkWidget *fileMenu;
    GtkWidget *openMenuItem;
    GtkWidget *saveMenuItem;
    GtkWidget *closeMenuItem;
                            
    rootFileItem = gtk_menu_item_new_with_mnemonic("_File");
    fileMenu = gtk_menu_new();
    openMenuItem = gtk_menu_item_new_with_label("Open");
    saveMenuItem = gtk_menu_item_new_with_label("Save");
    closeMenuItem = gtk_menu_item_new_with_label("Close");

    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMenuItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), 
    gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), closeMenuItem);
                                                                
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootFileItem), fileMenu);

    g_signal_connect(GTK_OBJECT(openMenuItem), "activate", G_CALLBACK(itemPressed), "Open ....");
    g_signal_connect(GTK_OBJECT(saveMenuItem), "activate", G_CALLBACK(itemPressed), "Save ....");
    g_signal_connect(GTK_OBJECT(closeMenuItem), "activate", G_CALLBACK(gtk_main_quit), NULL);

    return rootFileItem;
}

int main(int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menubar;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkMenuBar");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    menubar = gtk_menu_bar_new();

    gtk_menu_bar_append(menubar, createFileMenuItem());
    gtk_menu_bar_append(menubar, gtk_menu_item_new_with_mnemonic("_Edit"));
    gtk_menu_bar_append(menubar, gtk_menu_item_new_with_mnemonic("_About"));

    vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 2);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


