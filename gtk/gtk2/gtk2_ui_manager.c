/*************************************************************************
> FileName: gtk2_ui_manager.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月07日 星期五 15时42分55秒
 ************************************************************************/
#include <gtk/gtk.h>

void itemPressed(GtkMenuItem *menuItem, gpointer data)
{
    g_print("%s\n", gtk_action_get_name(GTK_ACTION(menuItem)));
}

static GtkActionEntry entries[] = {
    { "File", NULL, "_File"                                                                     },
    { "Open", GTK_STOCK_OPEN, "Open", "<control>O", "Open File", G_CALLBACK(itemPressed)        },
    { "Save", GTK_STOCK_SAVE, "Save",  "<control>S", "Save File", G_CALLBACK(itemPressed)       },
    { "Close", GTK_STOCK_QUIT, "Close",  "<control>Q", "Close File", G_CALLBACK(gtk_main_quit)  },
    { "Edit", NULL, "_Edit"                                                                     },
    { "Cut", NULL, "Copy"                                                                       },
    { "Copy", NULL, "Copy"                                                                      },
    { "Paste", NULL, "Paste"                                                                    },
    { "Help", NULL, "_Help"                                                                     },
    { "About", NULL, "About"                                                                    }
};

int main(int argc, char *argv[]) 
{
    GtkWidget *vbox = NULL;
    GtkUIManager *ui = NULL;
    GtkWidget *window = NULL;
    GtkActionGroup *actionGroup = NULL;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GtkUIManager");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    actionGroup = gtk_action_group_new("Actions");
    gtk_action_group_add_actions(actionGroup, entries, 10, NULL);
                                               
    ui = gtk_ui_manager_new();
    gtk_ui_manager_insert_action_group(ui, actionGroup, 0);
    gtk_ui_manager_add_ui_from_file(ui, "resource/gtk2_ui_manager.xml", NULL);

    vbox = gtk_vbox_new(FALSE, 5);

    // 此处路径是由 /ul 开始计算的
    gtk_box_pack_start(GTK_BOX(vbox), gtk_ui_manager_get_widget(ui, "/MenuBar"), FALSE, FALSE, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

