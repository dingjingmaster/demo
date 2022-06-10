/*************************************************************************
> FileName: gio-samba.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月08日 星期一 16时06分29秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

void mount_enclosing_volume_callback(GFile *volume, GAsyncResult *res, gpointer a);
void ask_question_cb(GMountOperation *op, char *message, char **choices, gpointer a);
void ask_password_cb(GMountOperation *op, const char *message, const char *default_user, const char *default_domain, GAskPasswordFlags flags, gpointer);


/* 匿名登录 */
int main (int argc, char* argv[])
{
    GFile*                      mvolume = NULL;
    GMountOperation*            mop = NULL;
    GCancellable*               mcancel = NULL;

    gtk_init (&argc, &argv);

    mcancel = g_cancellable_new();
    mop = g_mount_operation_new();
    mvolume = g_file_new_for_uri ("smb://172.30,50.191");

    g_mount_operation_set_username(mop, NULL);
    g_mount_operation_set_password(mop, NULL);
    g_mount_operation_set_domain(mop, NULL);
    g_mount_operation_set_anonymous(mop, TRUE);
    g_mount_operation_set_password_save(mop, G_PASSWORD_SAVE_FOR_SESSION);

    printf ("uri:%s\n", g_file_get_uri (mvolume));
    g_file_mount_enclosing_volume(mvolume, 0, mop, mcancel, mount_enclosing_volume_callback, NULL);

    g_signal_connect (mop, "ask-question", G_CALLBACK(ask_question_cb), NULL);
    g_signal_connect (mop, "ask-password", G_CALLBACK(ask_password_cb), NULL);

    gtk_main ();


    // free

    return 0;
}

void mount_enclosing_volume_callback(GFile *volume, GAsyncResult *res, gpointer a)
{
    GError *err = NULL;
    g_file_mount_enclosing_volume_finish (volume, res, &err);

    if (err) {
        printf ("error code:%d, error string:%s\n", err->code, err->message);
        g_error_free (err);
    } else {
        printf ("successful!!!\n");
    }
}

void ask_question_cb(GMountOperation *op, char *message, char **choices, gpointer a)
{
    printf ("询问...\n");
    char **choice = choices;
    int i = 0;
    while (*choice) {
        printf ("询问%s\n", *choice);
        g_mount_operation_set_choice(op, i);
        *choice++;
        i++;
    }
    g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
}

void ask_password_cb(GMountOperation *op, const char *message, const char *default_user, const char *default_domain, GAskPasswordFlags flags, gpointer a)
{
    printf ("询问密码!\n");
    g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
}
