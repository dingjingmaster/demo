/*************************************************************************
> FileName: gio-volume-monitor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月14日 星期六 20时05分04秒
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>

static void do_action(char* cmd)
{
    g_print("[do_action] start to exec: %s\n", cmd);
    GError* error = NULL;
    g_spawn_command_line_sync(cmd, NULL, NULL, NULL, &error);
    if (error) {
        g_print("[do_action] exec '%s' failed: %s\n", cmd, error->message);
        g_error_free(error);
    }
    g_print("[do_action] exec done\n");
}

static void handle_volume_changed(GVolumeMonitor* monitor, GVolume* volume, gpointer data)
{
    char* ev = (char*)data;
    g_print("[handle_volume_changed] event: %s\n", ev);
    if (0 == g_strcmp0(ev, "volume-removed")) {
        return;
    }

    g_autofree char* path = g_volume_get_identifier(volume, G_DRIVE_IDENTIFIER_KIND_UNIX_DEVICE);
    if (path) g_print("unix id: %s\n", path);
    g_autofree char* cmd = g_strdup_printf("gvfs-mount -d %s", path);

    do_action(cmd);
}

static void print_standard_info (GFile* file)
{
    g_autoptr(GFileInfo) fileInfo = g_file_query_info (file, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);

    printf ("target-uri: %s\n", g_file_info_get_attribute_string(fileInfo, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI));
}

static void handle_mount_changed(GVolumeMonitor* monitor, GMount* mount, gpointer data)
{
    char* ev = (char*)data;
    g_print("[handle_mount_changed] event: %s\n", ev);

    g_autoptr(GFile) file = g_mount_get_default_location(mount);
    g_autofree char* path = g_file_get_path(file);

    g_autoptr(GFile) file1 = g_file_new_for_path (path);
    g_autofree char* uri = g_file_get_uri(file1);
    g_autofree char* schema = g_file_get_uri_scheme(file1);

    printf ("path: %s\n", path);
    printf ("uri: %s\n", uri);
    printf ("schema: %s\n", schema);

    //do_action(cmd);
}

 

int main()
{
    GVolumeMonitor* monitor = g_volume_monitor_get();

    g_signal_connect(G_OBJECT(monitor), "volume-added", G_CALLBACK(handle_volume_changed), "volume-added");
    g_signal_connect(G_OBJECT(monitor), "volume-removed", G_CALLBACK(handle_volume_changed), "volume-removed");

    g_signal_connect(G_OBJECT(monitor), "mount-added", G_CALLBACK(handle_mount_changed), "mount-added");
    g_signal_connect(G_OBJECT(monitor), "mount-removed", G_CALLBACK(handle_mount_changed), "mount-removed");
 
    g_main_loop_run(g_main_loop_new(0,0));

    return 0;
}

