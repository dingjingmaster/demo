/*************************************************************************
> FileName: gio-mount-smb.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年06月15日 星期一 10时16分48秒
 ************************************************************************/
#include <glib-2.0/glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <stdio.h>

static void async_ready_cb(GObject *object, GAsyncResult *res, GtkBuilder *builder)
{
	g_message ("async_ready_cb");
	GFile *file = G_FILE (object);
	GError *err = NULL;
	g_file_mount_enclosing_volume_finish (file, res, &err);

	if (err){
		g_message ("%s",err->message);
		
		GtkLabel *status_label = GTK_LABEL (gtk_builder_get_object(builder, "status_hint_label"));
		gtk_label_set_text (status_label, err->message);
		
		g_error_free (err);
	}

	GFileEnumerator *enumerator = g_file_enumerate_children(file, G_FILE_ATTRIBUTE_STANDARD_NAME, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, NULL);

	GtkWidget *list_box = gtk_list_box_new ();

	if (enumerator) {
		int count = 0;
		GFileInfo *info;
		while ((info = g_file_enumerator_next_file (enumerator, NULL, NULL)) != NULL) {
			g_message ("%s",g_file_info_get_name (info));
			count++;

			GtkWidget *name_label = gtk_label_new (g_file_info_get_name (info));
			gtk_list_box_prepend (GTK_LIST_BOX (list_box), name_label);
			g_object_unref(info);
		}

		if (count > 0) {
			GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_container_add (GTK_CONTAINER (window), list_box);
			gtk_widget_show_all (window);
			g_message ("\n%d files", count);
		} else {
			g_object_unref (list_box);
		}
		g_object_unref(enumerator);
	}
}

static void ask_question_cb (GMountOperation *op, char *message, char **choices, GtkBuilder* builder)
{
    char **ptr = choices;
    char *s;
    int i, choice;

    g_print ("%s\n", message);

    i = 1;
    while (*ptr) {
        g_print ("[%d] %s\n", i, *ptr++);
        i++;
    }
    g_mount_operation_set_choice (op, 0);
    g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
}

static void ask_password_cb (GMountOperation *op, const char *message, const char *default_user, const char *default_domain, GAskPasswordFlags flags, GtkBuilder *builder) 
{
	g_message ("ask_password_cb");

	GtkEntry *username_entry = GTK_ENTRY (gtk_builder_get_object (builder, "username_entry"));
	GtkEntry *pwd_entry = GTK_ENTRY (gtk_builder_get_object (builder, "pwd_entry"));
	const char* user  = gtk_entry_get_text (username_entry);
	const char* pwd  = gtk_entry_get_text (pwd_entry);

	g_message ("msg: %s\n", message);
	if (default_user) {
		g_message ("user: %s", default_user);
	}
	if (default_domain) {
		g_message ("domain: %s", default_domain);
	}

    if (flags & G_ASK_PASSWORD_NEED_USERNAME)
	{
		g_message ("need username");
		g_mount_operation_set_username (op, user);
    }

	if (flags & G_ASK_PASSWORD_NEED_PASSWORD)
	{
		g_message ("need pwd");
		g_mount_operation_set_password (op, pwd);
    }
	
	if (flags & G_ASK_PASSWORD_NEED_DOMAIN) {
		g_message ("need domain");
	}

	g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
}

static void aborted_cb (GMountOperation *op, GtkBuilder *builder)
{
	g_message ("aborted_cb");
}

static void login_bt_cb (GtkWidget *button, GtkBuilder *builder)
{
	GtkEntry *host_entry = GTK_ENTRY (gtk_builder_get_object (builder, "host_entry"));
	GtkEntry *username_entry = GTK_ENTRY (gtk_builder_get_object (builder, "username_entry"));
	GtkEntry *pwd_entry = GTK_ENTRY (gtk_builder_get_object (builder, "pwd_entry"));
	const char* url = gtk_entry_get_text (host_entry);
	const char* user  = gtk_entry_get_text (username_entry);
	const char* pwd  = gtk_entry_get_text (pwd_entry);
	g_message ("login:%s, as user %s", url, user);
	
	GMountOperation *mount_op = g_mount_operation_new ();
	g_mount_operation_set_username (mount_op, user);
	g_mount_operation_set_password (mount_op, pwd);
	GFile *remote_file = g_file_new_for_uri (url);
	g_file_mount_enclosing_volume (remote_file,
			G_MOUNT_MOUNT_NONE,
			mount_op,
			NULL,
			(GAsyncReadyCallback) async_ready_cb,
			builder);

	g_signal_connect (mount_op, "ask-question", G_CALLBACK(ask_question_cb), builder);
	g_signal_connect (mount_op, "ask-password", G_CALLBACK (ask_password_cb), builder);
	g_signal_connect (mount_op, "aborted", G_CALLBACK (aborted_cb), builder);
}

static void reset_bt_cb (GtkWidget *button, GtkBuilder *builder)
{
	GtkEntry *host_entry = GTK_ENTRY (gtk_builder_get_object (builder, "host_entry"));
	GtkEntry *username_entry = GTK_ENTRY (gtk_builder_get_object (builder, "username_entry"));
	GtkEntry *pwd_entry = GTK_ENTRY (gtk_builder_get_object (builder, "pwd_entry"));
	GtkLabel *status_label = GTK_LABEL (gtk_builder_get_object(builder, "status_hint_label"));
	gtk_entry_set_text (host_entry, "");
	gtk_entry_set_text (username_entry, "");
	gtk_entry_set_text (pwd_entry, "");
	gtk_label_set_text (status_label, "dialog has been reseted");

}

int main ()
{
	gtk_init(NULL, NULL);

	GtkBuilder *builder = gtk_builder_new_from_file ("remote-login.ui");
	GtkWidget *top_window = GTK_WIDGET (gtk_builder_get_object (builder, "login_dialog"));

	GtkWidget *button_box = GTK_WIDGET (gtk_builder_get_object (builder, "button_box"));

	GtkWidget *login_button = gtk_button_new_with_label ("Login");
	gtk_container_add (GTK_CONTAINER (button_box), login_button);
	GtkWidget *reset_button = gtk_button_new_with_label ("Reset");
	gtk_container_add (GTK_CONTAINER (button_box), reset_button);

	g_signal_connect (GTK_BUTTON (login_button), "clicked", G_CALLBACK (login_bt_cb), builder);
	g_signal_connect (GTK_BUTTON (reset_button), "clicked", G_CALLBACK (reset_bt_cb), builder);

	g_signal_connect (top_window, "destroy", gtk_main_quit, NULL);

	gtk_widget_show_all (top_window);

	gtk_main ();
	return 0;
}
