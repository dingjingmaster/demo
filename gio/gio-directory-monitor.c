/*************************************************************************
> FileName: gio-directory-monitor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月28日 星期三 14时15分29秒
 ************************************************************************/
#include <glib.h>
#include <gio/gio.h>

static void handle(GFileMonitor     *monitor,
                   GFile            *file,
                   GFile            *other_file,
                   GFileMonitorEvent event_type,
                   gpointer          user_data)
{
    printf("change ...\n");
    switch (event_type) {
        case G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED:
        case G_FILE_MONITOR_EVENT_CHANGED: {
            char* uri = g_file_get_uri(file);
            printf ("file %s changed!", uri);
            if (NULL != uri) g_free(uri);
            break;
        }
        case G_FILE_MONITOR_EVENT_CREATED: {
            char* uri = g_file_get_uri(file);
            printf ("file %s created!", uri);
            if (NULL != uri) g_free(uri);
            break;
        }
        case G_FILE_MONITOR_EVENT_DELETED: {
            char* uri = g_file_get_uri(file);
            printf ("file %s deleted!", uri);
            if (NULL != uri) g_free(uri);
            break;
        }
        case G_FILE_MONITOR_EVENT_UNMOUNTED: {
            char* uri = g_file_get_uri(file);
            printf ("file %s unmounted!!", uri);
            if (NULL != uri) g_free(uri);
            break;
        }
        default:
            break;
    }
}

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf("请输入要监听的文件夹uri!\n");
        return -1;
    }

    char* path = argv[1];
    GFile* file = NULL;
    GError* error = NULL;
    GFileMonitor* fileMonitor = NULL;

    // g_file_monitor_file
    file = g_file_new_for_uri (path);
    printf("monitor dir: %s\n", g_file_get_uri(file));
    fileMonitor = g_file_monitor_directory (file, G_FILE_MONITOR_NONE, NULL, &error);
    if (NULL != error) {
        printf ("error:%s\n", error->message);
        goto out;
    }

    g_signal_connect (G_OBJECT(fileMonitor), "changed", G_CALLBACK(handle), NULL);

    g_main_loop_run(g_main_loop_new(0,0));

out:
    if (NULL != error)          g_error_free(error);
    if (NULL != file)           g_object_unref(file);
    if (NULL != fileMonitor)    g_object_unref(fileMonitor);

    return 0;
}

