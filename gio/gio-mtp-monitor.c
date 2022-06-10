/*************************************************************************
> FileName: gio-mtp-monitor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年10月28日 星期四 10时32分37秒
 ************************************************************************/

#include <gio/gio.h>

static void print_volume_info   (GVolume* volume); 
static void print_file_info     (GFileInfo* fileInfo);
static void connected_volumes   (GVolumeMonitor* monitor);
static void handle_mount_changed(GVolumeMonitor* monitor, GMount* mount, gpointer data);
static void handle_volume_changed(GVolumeMonitor* monitor, GVolume* volume, gpointer data);
static GAsyncReadyCallback mount_volume_callback (GFile *volume, GAsyncResult *res, gpointer udata);
static GAsyncReadyCallback umount_volume_callback (GFile* file, GAsyncResult* res, gpointer udata);
static GAsyncReadyCallback query_fileinfo_callback (GFile* file, GAsyncResult* res, gpointer udata);


int main (int argc, char* argv[])
{
    GVolumeMonitor* monitor = g_volume_monitor_get();

    connected_volumes (monitor);

    //g_signal_connect(G_OBJECT(monitor), "volume-added",     G_CALLBACK(handle_volume_changed),  "volume-added");
    //g_signal_connect(G_OBJECT(monitor), "volume-removed",   G_CALLBACK(handle_volume_changed),  "volume-removed");

    g_signal_connect(G_OBJECT(monitor), "mount-added",      G_CALLBACK(handle_mount_changed),   "mount-added");
    g_signal_connect(G_OBJECT(monitor), "mount-removed",    G_CALLBACK(handle_mount_changed),   "mount-removed");
 
    g_main_loop_run(g_main_loop_new(0,0));

    return 0;
}

static void print_volume_info (GVolume* volume)
{
    g_return_if_fail (G_IS_VOLUME (volume));

    g_autofree gchar* name = g_volume_get_name (volume);
    g_autofree gchar* devName = g_volume_get_identifier (volume, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
    g_autoptr (GFile) file = g_volume_get_activation_root (volume);

    if (G_IS_FILE (file)) {
        g_autofree gchar* uri = g_file_get_uri (file);
        if (g_str_has_prefix (uri, "mtp://")) {
            printf ("\e[41m [device]: %-30s  [Name]: %-30s [URI]: %-60s \e[0m\n", devName, name, uri);
        } else {
            printf ("[device]: %-30s  [Name]: %-30s [URI]: %-60s\n", devName, name, uri);
        }
        //g_file_mount_enclosing_volume (file, G_MOUNT_MOUNT_NONE, NULL, NULL, mount_volume_callback, NULL);

        g_autoptr (GError) error1 = NULL;
        g_file_query_info_async (file, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, 0, NULL, query_fileinfo_callback, &error1);
        if (error1) {
            printf ("\t\t %d -- %s\n ", error1->code, error1->message);
        } 
    }
}

static void print_file_info (GFileInfo* fileInfo)
{
    g_return_if_fail (G_IS_FILE_INFO (fileInfo));

    int i = 0;
    gchar** attrs = g_file_info_list_attributes (fileInfo, NULL);
    while (attrs && attrs[i]) {
        printf ("\t [%-50s]: %s\n", attrs[i], g_file_info_get_attribute_as_string (fileInfo, attrs[i]));
        ++i;
    }
}

static void connected_volumes (GVolumeMonitor* monitor)
{
    g_return_if_fail (G_IS_VOLUME_MONITOR(monitor));

    GList* vs = NULL;
    GList* volumes = NULL;
    
    volumes = vs = g_volume_monitor_get_volumes (monitor);
    while (vs) {
        GVolume* v = G_VOLUME (vs->data);
        print_volume_info (v);
        vs = vs->next;
    }

    if (volumes) g_list_free_full (volumes, g_object_unref);
}

static void handle_volume_changed (GVolumeMonitor* monitor, GVolume* volume, gpointer data)
{
    if (0 == g_strcmp0(data, "volume-removed")) {
        return;
    }

    g_return_if_fail (G_IS_VOLUME (volume));

    if (G_IS_VOLUME (volume)) {
        print_volume_info (volume);
    }
}
static GAsyncReadyCallback query_fileinfo_callback (GFile* file, GAsyncResult* res, gpointer udata)
{
    g_autoptr (GError) error = NULL;
    g_autoptr (GFileInfo) fileinfo = g_file_query_info_finish (file, res, &error);
    if (error) {
        printf ("\t\t error: %d --- %s\n", error->code, error->message);
        return NULL;
    }
    print_file_info (fileinfo);
}

static void handle_mount_changed(GVolumeMonitor* monitor, GMount* mount, gpointer data)
{
    char* ev = (char*)data;
    if (g_strcmp0(ev, "mount-removed") == 0) {
        return;
    }

    g_return_if_fail (G_IS_MOUNT (mount));

    g_autoptr (GVolume) volume = g_mount_get_volume (mount);

    if (G_IS_VOLUME (volume)) {
        print_volume_info (volume);
    }
}

static GAsyncReadyCallback mount_volume_callback (GFile* file, GAsyncResult* res, gpointer udata)
{
    g_autoptr (GError) error = NULL;
    g_file_mount_enclosing_volume_finish (file, res, &error);
    if (!error || (error->code == G_IO_ERROR_ALREADY_MOUNTED)) {
        g_autoptr (GError) error1 = NULL;
        g_autoptr (GFileInfo) fileInfo = g_file_query_info (file, "*", G_FILE_QUERY_INFO_NONE, NULL, &error1);
        if (error1) {
            printf ("\t\t %d -- %s\n ", error1->code, error1->message);
        } else {
            print_file_info (fileInfo);
        }

        return NULL;
    }
    
    if (error)  printf ("\t\terror: %d --- %s\n", error->code, error->message);

    switch (error->code) {
    case G_IO_ERROR_CANCELLED:
    case G_IO_ERROR_EXISTS:
        g_file_unmount_mountable_with_operation (file, G_MOUNT_UNMOUNT_FORCE, NULL, NULL, umount_volume_callback, NULL);
        break;
    default:
        break;
    }
}

static GAsyncReadyCallback umount_volume_callback (GFile* file, GAsyncResult* res, gpointer udata)
{
    g_autoptr (GError) error = NULL;
    g_file_unmount_mountable_with_operation_finish (file, res, &error);
    
    if (error)  printf ("\t\terror: %d --- %s\n", error->code, error->message);
}
