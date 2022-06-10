/*************************************************************************
> FileName: test-polkit-demo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 10 Jun 2022 05:06:48 PM CST
 ************************************************************************/

#include <stdio.h>
#include <gio/gio.h>
#include <polkit/polkit.h>

static gboolean on_tensec_timeout (gpointer user_data)
{
    GMainLoop *loop = user_data;
    g_print ("Ten seconds has passed. Now exiting.\n");
    g_main_loop_quit (loop);

    return FALSE;
}

static void check_authorization_cb (PolkitAuthority* authority, GAsyncResult* res, gpointer user_data)
{
    GMainLoop *loop = user_data;
    PolkitAuthorizationResult *result;
    GError *error;

    error = NULL;
    result = polkit_authority_check_authorization_finish (authority, res, &error);
    if (error != NULL) {
        g_print ("Error checking authorization: %s\n", error->message);
        g_error_free (error);
    } else {
        const gchar *result_str; 
        if (polkit_authorization_result_get_is_authorized (result)) {
            result_str = "authorized";
        } else if (polkit_authorization_result_get_is_challenge (result)) { 
            result_str = "challenge"; 
        } else { 
            result_str = "not authorized"; 
        }

        g_print ("Authorization result: %s\n", result_str);
    }

    g_print ("取消,进程将要推出.\n");
    g_timeout_add (10000, on_tensec_timeout, loop);
}

static gboolean do_cancel (GCancellable *cancellable)
{
    g_print ("检测超时，退出权限验证\n");
    g_cancellable_cancel (cancellable);
   
    return FALSE;
}

int main (int argc, char *argv[])
{
    pid_t parent_pid;
    const gchar *action_id;
    GMainLoop *loop;
    PolkitSubject *subject;
    PolkitAuthority *authority;
    GCancellable *cancellable;

    action_id = "org.demo.pkexec.policy.pk-demo";

    loop = g_main_loop_new (NULL, FALSE);

    authority = polkit_authority_get_sync (NULL, NULL);

    parent_pid = getppid ();
    if (parent_pid == 1) {
        g_printerr ("Parent process was reaped by init(1)\n");
        return 1;
    }

    printf ("pid:%d\n", getpid());

    subject = polkit_unix_process_new_for_owner (parent_pid, 0, getuid ());

    cancellable = g_cancellable_new ();

    g_print ("Will cancel authorization check in 10 seconds\n");

    /* Set up a 10 second timer to cancel the check */
    g_timeout_add (10 * 1000, (GSourceFunc) do_cancel, cancellable);

    polkit_authority_check_authorization (authority, subject, action_id, NULL, POLKIT_CHECK_AUTHORIZATION_FLAGS_ALLOW_USER_INTERACTION, cancellable, (GAsyncReadyCallback) check_authorization_cb, loop);

    g_main_loop_run (loop);

    g_object_unref (authority);
    g_object_unref (subject);
    g_object_unref (cancellable);
    g_main_loop_unref (loop);

    return 0;
}

