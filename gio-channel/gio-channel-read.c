/*************************************************************************
> FileName: gio-channel-read.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月22日 星期二 14时06分37秒
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gmodule.h>

static gboolean io_watch_stdout_custom (GIOChannel* source, GIOCondition condition, gpointer data);

int main (int argc, char* argv[])
{
    gchar* envp[1];
    gchar* argvs[4];
    GError* error;
    GPid backend_pid;
    gint my_stdin, my_stdout, my_stderr;
    GIOChannel* backend_stdin = NULL;
    GIOChannel* backend_stdout = NULL;

    argv[0] = "echo 'aaaa'";
    
    if (!g_spawn_async_with_pipes (NULL, argvs, envp, G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, 
                &backend_pid, &my_stdin, &my_stdout, &my_stderr, &error)) {
        return -1;
    }

    backend_stdin = g_io_channel_unix_new (my_stdin);
    backend_stdout = g_io_channel_unix_new (my_stdout);

    // set encoding
    if (g_io_channel_set_encoding(backend_stdin, NULL, &error) != G_IO_STATUS_NORMAL
            || g_io_channel_set_encoding (backend_stdout, NULL, &error) != G_IO_STATUS_NORMAL
            || g_io_channel_set_flags (backend_stdin, G_IO_FLAG_NONBLOCK, &error) != G_IO_STATUS_NORMAL
            || g_io_channel_set_flags (backend_stdout, G_IO_FLAG_NONBLOCK, &error) != G_IO_STATUS_NORMAL) {
        printf ("error:%s", error->message);
        return -1;
    }

    // Turn off buffering
    g_io_channel_set_buffered (backend_stdin, FALSE);
    g_io_channel_set_buffered (backend_stdout, FALSE);

    // add io watcher
    g_io_add_watch (backend_stdout, G_IO_IN | G_IO_PRI, (GIOFunc) io_watch_stdout_custom, NULL);

    // add child watcher
    //g_child_watch_add (backend_pid, (GChildWatchFunc) child_watch_cb, NULL);

    return 0;
}

static gboolean io_watch_stdout_custom (GIOChannel* source, GIOCondition condition, gpointer data)
{
    static GString*         str = NULL;
    GError*                 error = NULL;
    gsize                   bytes_read = 0;
    gchar                   buf[1024] = {0};

    if (NULL == str) {
        g_string_new ("AAA");
    }

    if (g_io_channel_read_chars (source, buf, 1024, &bytes_read, &error) != G_IO_STATUS_NORMAL) {
        printf ("error:%s\n", error->message);
        return FALSE;
    }

    printf ("---->%s\n", buf);

    return TRUE;
    
}
