#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>



static gboolean prompt_dialog (gchar* action) {
    int ret = FALSE;
    int logged_user = 2;
    gchar* prompt = NULL;
    GtkWidget* dialog = NULL;
    const GList *items, *item;

//    items = lightdm_user_list_get_users (lightdm_user_list_get_instance ());
//    for (item = items; item; item = item->next) {
//        LightDMUser *user = item->data;
//        if (lightdm_user_get_logged_in (user))logged_user++;
//    }

    if (logged_user > 1) {
        prompt = g_strdup_printf ("\n\nThere are still %d users logged in. Confirm %s", logged_user, action);
    } else if (logged_user == 1) {
        prompt = g_strdup_printf ("\n\nThere is still %d users logged in. Confirm %s", logged_user, action);
    } else {
        prompt = g_strdup_printf ("\n\nConfirm %s", action);
    }

    dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK_CANCEL,
                prompt);
//    gtk_widget_set_app_paintable(dialog, TRUE);
    GtkWidget* image=gtk_image_new_from_file("../warning_bg.png");
    gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_default_size(dialog, 300, 200);

    gtk_message_dialog_set_image (dialog, image);


//    gtk_widget_hide (GTK_WIDGET (panel_window));
    gtk_widget_show_all (dialog);
//    center_window (GTK_WINDOW (dialog), NULL, &CENTERED_WINDOW_POS);

    ret = gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK ? TRUE : FALSE;
    g_free(prompt);
    gtk_widget_destroy(dialog);
//    gtk_widget_show (GTK_WIDGET (panel_window));
    return ret;
}

int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);

    if (prompt_dialog("是否关机?")) {
        printf("关机");
    } else {
        printf("取消");
    }

    gtk_main();
    return 0;
}
