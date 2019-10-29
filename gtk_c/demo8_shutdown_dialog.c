#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <djctool/clib_syslog.h>

static int  te_connect_dialog(void* session)
{
    GtkWidget *dialog;
    int retval;
    dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK,
                "\n虚拟机连接异常，请重启虚拟机\n");

    gtk_window_set_title(GTK_WINDOW(dialog), "Warning");

    // 窗体大小不可调整
    gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);

    // 设置背景图片
    GtkWidget* bg = gtk_image_new_from_file("warning_bg.png");
    gtk_container_add (GTK_CONTAINER (dialog), bg);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 0);

    gtk_dialog_run(GTK_DIALOG(dialog));
    retval=-1;
    gtk_widget_destroy(dialog);
    return retval;
}

int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);

    if (te_connect_dialog(NULL)) {
        CT_SYSLOG(LOG_INFO, "asdasdasd");
    }

    gtk_main();
    return 0;
}
