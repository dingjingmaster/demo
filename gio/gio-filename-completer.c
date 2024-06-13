/*************************************************************************
> FileName: gio-filename-completer.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 14 Mar 2023 04:17:39 PM CST
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

static void handle (GFilenameCompleter* fc, gpointer);

int main (int argc, char* argv[])
{
    g_autoptr(GFilenameCompleter) fc = g_filename_completer_new ();

    g_autofree char* s = g_filename_completer_get_completion_suffix (fc, "txt");
    printf ("%s\n", s ? s : "<null>");

    char** arr = g_filename_completer_get_completions (fc, "/d");

    for (int i = 0; arr[i] != NULL; ++i) {
        printf ("%s\n", arr[i]);
    }

    g_signal_connect(G_OBJECT(fc), "got-completion-data", G_CALLBACK(handle), NULL);

    g_main_loop_run(g_main_loop_new(0, 0));

    return 0;
}

static void handle (GFilenameCompleter* fc, gpointer d)
{
    char** arr = g_filename_completer_get_completions (fc, "a");
    for (int i = 0; arr[i] != NULL; ++i) {
        printf ("%s\n", arr[i]);
    }
}
