/*************************************************************************
> FileName: gio-copy-progress.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月26日 星期六 17时30分33秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>

void progress_callback(goffset current_num_bytes, goffset total_num_bytes, void* data)
{
    while (1);
}

int main (int argc, char* argv[])
{
    if (argc < 3) {
        printf ("input error, please input: src, dest\n");
        return -1;
    }

    GError*                 error = NULL;
    g_autoptr(GFile)        src = g_file_new_for_path(argv[1]);
    g_autoptr(GFile)        dest = g_file_new_for_path(argv[2]);
    g_autoptr(GCancellable) cancel = g_cancellable_new();

    //
    g_file_copy(src, dest, G_FILE_COPY_NOFOLLOW_SYMLINKS, cancel, progress_callback, NULL, &error);
    if (NULL != error) {
        printf ("error: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    return 0;
}
