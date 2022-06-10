#include <glib.h>
#include <gio/gio.h>
 
static void
do_action(char* cmd)
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
 
static void
handle_volume_changed(GVolumeMonitor* monitor, GVolume* volume, gpointer data)
{
    char* ev = (char*)data;
    g_print("[handle_volume_changed] event: %s\n", ev);
    return;
 
    if (g_strcmp0(ev, "volume-removed") == 0) {
        return;
    }
 
    char* path = g_volume_get_identifier(volume, "unix-device");
    char* cmd = g_strdup_printf("gvfs-mount -d %s", path);
    g_free(path);
    do_action(cmd);
    g_free(cmd);
}
 
static void
handle_mount_changed(GVolumeMonitor* monitor, GMount* mount, gpointer data)
{
    char* ev = (char*)data;
    g_print("[handle_mount_changed] event: %s\n", ev);
    return;
    if (g_strcmp0(ev, "mount-removed") == 0) {
        return;
    }
 
    GFile* root = g_mount_get_root(mount);
    char* path = g_file_get_path(root);
    g_object_unref(G_OBJECT(root));
    char* cmd = g_strdup_printf("gvfs-open %s", path);
    g_free(path);
    do_action(cmd);
    g_free(cmd);
}
 
int
main()
{
    GVolumeMonitor* monitor = g_volume_monitor_get();

    GList *mounts, *l;

    mounts = g_volume_monitor_get_mounts(monitor);

    for (l = mounts; l; l = l->next)
    {
      GMount *mount = l->data;
#if 0
      GVolume *volume;
      GDrive *drive;

      drive = g_mount_get_drive (mount);
      volume = g_mount_get_volume (mount);
      do_mount_tests (drive, volume, mount);

      if (drive != NULL)
        g_object_unref (drive);
      if (volume != NULL)
        g_object_unref (volume);
#endif
      GFile *file = g_mount_get_root(mount);

      char* path = g_file_get_path(file);
      g_print("path is %s\n", path);

      if (path != NULL)
          g_free(path);
      if (file != NULL)
          g_object_unref(file);
    }

    g_list_free_full(mounts, g_object_unref);

    g_signal_connect(G_OBJECT(monitor), "volume-added",
                     G_CALLBACK(handle_volume_changed), "volume-added");
    g_signal_connect(G_OBJECT(monitor), "volume-removed",
                     G_CALLBACK(handle_volume_changed), "volume-removed");
    g_signal_connect(G_OBJECT(monitor), "mount-added",
                     G_CALLBACK(handle_mount_changed), "mount-added");
    g_signal_connect(G_OBJECT(monitor), "mount-removed",
                     G_CALLBACK(handle_mount_changed), "mount-removed");
 
    g_main_loop_run(g_main_loop_new(0,0));
	return 0;
}
