/*************************************************************************
> FileName: gio-file-monitor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月20日 星期一 14时55分04秒
 ************************************************************************/
#include <glib.h>
#include <gio/gio.h>

static void handle(GFileMonitor     *monitor,
               GFile            *file,
               GFile            *other_file,
               GFileMonitorEvent event_type,
               gpointer          user_data)
{
    if (event_type == G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
    printf ("...\n");
}

int main (int argc, char* argv[])
{
    char* path = "./test_file.txt";
    GFile* file = NULL;
    GFileMonitor* fileMonitor = NULL;

    // g_file_monitor_file
    file = g_file_new_for_path (path);
    g_file_create (file, G_FILE_CREATE_NONE, NULL, NULL);
    fileMonitor = g_file_monitor_file (file, G_FILE_MONITOR_NONE, NULL, NULL);

    g_signal_connect (G_OBJECT(fileMonitor), "changed", G_CALLBACK(handle), NULL);

    g_main_loop_run(g_main_loop_new(0,0));

    g_object_unref(fileMonitor);
    g_object_unref(file);

    return 0;
}
