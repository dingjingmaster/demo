/*************************************************************************
> FileName: gio-subprocess.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 28 Apr 2022 11:30:10 AM CST
 ************************************************************************/
#include <gio/gio.h>

static void done (GObject* source, GAsyncResult* result, gpointer udata)
{
    if (!g_subprocess_communicate_utf8_finish (G_SUBPROCESS (source), result, NULL, NULL, NULL)) {
        printf ("subprocess communicate error!\n");
    } else {
        if (!g_subprocess_get_successful (G_SUBPROCESS (source))) {
            printf ("error!\n");
        } else {
            printf ("ok!\n");
        }
    }
}

int main (int argc, char* argv[])
{
    char* argv1[] = {
        "touch",
        "/home/dingjing/kkk.txt",
        NULL
    };

    g_autoptr(GError) error = NULL;

    GSubprocess* proc = g_subprocess_newv (argv1, G_SUBPROCESS_FLAGS_STDOUT_SILENCE | G_SUBPROCESS_FLAGS_STDERR_PIPE, &error);
    g_assert_no_error (error);

    g_subprocess_communicate_utf8_async (proc, NULL, NULL, NULL, NULL);

    if (error) {
        printf ("error: %s\n", error->message);
    }

    return 0;
}
