#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static char* user_num_tmp = "/tmp/user_num";

static int user_num() {
    char buf[32] = {0};
    int usernum = 0;

    system("who | awk '{print $1}' | sort | uniq | wc -l > /tmp/user_num");
    int fd = open(user_num_tmp, O_RDWR | O_CREAT);
    if (fd < 0) {
        return -1;
    }
    int iret = flock(fd, LOCK_EX | LOCK_NB);
    if (iret < 0) {
        return -1;
    }

    if (read(fd, buf, sizeof buf) < 0) {
        return -1;
    }

    iret = flock(fd, LOCK_UN);
    if (iret < 0) {
        return -1;
    }
    remove(user_num_tmp);
    usernum = atoi(buf);
    if (usernum < 0) {
        usernum = -1;
    }
    return usernum;
}


static gboolean prompt_dialog (gchar* action) {
    int ret = FALSE;
    int logged_user = 2;
    gchar* prompt = NULL;
    GtkWidget* dialog = NULL;
    GtkWidget* content = NULL;
    GtkWidget* label = NULL;
    GtkWidget* cancel = NULL;
    GtkWidget* ok = NULL;

    logged_user = user_num();

    if (logged_user > 1) {
        prompt = g_markup_printf_escaped ("\n\nThere are still %d users logged in.\n\nConfirm %s", logged_user, action);
    } else if (logged_user == 1) {
        prompt = g_markup_printf_escaped ("\n\nThere is still %d users logged in.\n\nConfirm %s", logged_user, action);
    } else if (0 == logged_user) {
        prompt = g_markup_printf_escaped ("\n\nConfirm %s", action);
    } else {
        prompt = g_markup_printf_escaped ("\n\nAt least one person is using.\n\nConfirm %s", action);
    }

    dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK_CANCEL,
                "%s", prompt);

    dialog = gtk_dialog_new ();

    gtk_window_set_default_size(GTK_WINDOW(dialog), 490, 200);
    gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
    gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new (prompt);
    gtk_container_add (GTK_CONTAINER (content), label);

    cancel = gtk_dialog_add_button (GTK_DIALOG (dialog), "Cancel", GTK_RESPONSE_CANCEL);
    ok = gtk_dialog_add_button (GTK_DIALOG (dialog), "OK", GTK_RESPONSE_OK);

    GtkCssProvider *provider = provider = gtk_css_provider_new ();
    GdkDisplay *display = display = gdk_display_get_default ();
    GdkScreen *screen = screen = gdk_display_get_default_screen (display);
    gtk_css_provider_load_from_data (provider,
      "  dialog {\n"
      "     color: rgba(255, 255, 255, 100);\n"
      "     background-repeat: no-repeat;"
      "     background-position: center;"
      "     background-image: url('warning_bg.png');\n"
      "     background-color: rgba(55, 150, 246, 100);\n"
      " }\n"
      " button {\n"
      " }\n"
      , -1, NULL);

      gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
      g_object_unref (provider);

    gtk_widget_show_all (dialog);
    ret = gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK ? TRUE : FALSE;
    g_free(prompt);
    gtk_widget_destroy(dialog);
    return ret;
}

int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);

    if (prompt_dialog("是否关机?")) {
    } else {
    }

    gtk_main();
    return 0;
}
