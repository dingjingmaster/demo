/*************************************************************************
> FileName: gio-computer-monitor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 23 Nov 2022 10:05:03 AM CST
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

static void print_srandard_info (const GFile* file)
{
    g_autoptr(GFileInfo) fileInfo = g_file_query_info (file, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);

    g_autofree char* targetUri = g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI);

    //g_autoptr(GFile) ff = g_file_new_for_uri (targetUri);
    g_autoptr(GMount) mount = g_file_find_enclosing_mount (file, NULL, NULL);
    g_autoptr(GFile) mountPoint = g_mount_get_root (mount);

    printf ("uri: %s\n", g_file_get_uri(file));
    printf ("target-uri: %s\n", targetUri);
    printf ("mount uri: %s\n", g_file_get_uri(mountPoint));
}

static void handle_changed_event (GFileMonitor* m, GFile* f, GFile* fo, GFileMonitorEvent ev, gpointer udata)
{
    if (G_FILE_MONITOR_EVENT_CHANGED == ev)                     printf ("G_FILE_MONITOR_EVENT_CHANGED!\n");
    else if (G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT == ev)      printf ("G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT!\n");
    else if (G_FILE_MONITOR_EVENT_DELETED == ev)                printf ("G_FILE_MONITOR_EVENT_DELETED!\n");
    else if (G_FILE_MONITOR_EVENT_CREATED == ev)                printf ("G_FILE_MONITOR_EVENT_CREATED!\n");
    else if (G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED == ev)      printf ("G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED!\n");
    else if (G_FILE_MONITOR_EVENT_PRE_UNMOUNT == ev)            printf ("G_FILE_MONITOR_EVENT_PRE_UNMOUNT!\n");
    else if (G_FILE_MONITOR_EVENT_UNMOUNTED == ev)              printf ("G_FILE_MONITOR_EVENT_UNMOUNTED!\n");
    else if (G_FILE_MONITOR_EVENT_MOVED == ev)                  printf ("G_FILE_MONITOR_EVENT_MOVED!\n");
    else if (G_FILE_MONITOR_EVENT_RENAMED == ev)                printf ("G_FILE_MONITOR_EVENT_RENAMED!\n");
    else if (G_FILE_MONITOR_EVENT_MOVED_IN == ev)               printf ("G_FILE_MONITOR_EVENT_MOVED_IN!\n");
    else if (G_FILE_MONITOR_EVENT_MOVED_OUT == ev)              printf ("G_FILE_MONITOR_EVENT_MOVED_OUT!\n");

    if (f) print_srandard_info(f);
    if (fo)print_srandard_info(fo);
}

int main (int argc, char* argv[])
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GFile) file = g_file_new_for_uri ("computer:///");
    g_autoptr(GFileMonitor) fileMonitor = g_file_monitor(file, G_FILE_MONITOR_WATCH_MOUNTS, NULL, &error);

    g_signal_connect(fileMonitor, "changed", G_CALLBACK(handle_changed_event), NULL);


    g_main_loop_run(g_main_loop_new(0,0));

    return 0;
}
