/*************************************************************************
> FileName: gio-volume-can-mount.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月18日 星期五 18时27分44秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    GList* drives = NULL;
    GVolumeMonitor* vm = NULL;

    vm = g_volume_monitor_get();
    if (NULL == vm) {
        printf ("volume monitor error!\n");
        return -1;
    }

    drives = g_volume_monitor_get_connected_drives(vm);
    if (NULL == drives) {
        printf ("drive error!\n");
        return -1;
    }

    for (GList* l = drives; l != NULL; l = l->next) {
        GDrive* d = (GDrive*)(l->data);
        GList* volumes = g_drive_get_volumes(d);
        g_autofree char* driveName = g_drive_get_identifier (d, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
        printf ("%s\n", driveName);
        for (GList* d = volumes; d != NULL; d = d->next) {
            GVolume* vv = (GVolume*)(d->data);
            g_autofree char* devName = g_volume_get_identifier (vv, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
            if (NULL == devName) {
                continue;
            }
            printf ("  \\___%s  --  %s\n", devName, g_volume_can_mount(vv) ? "can mount!" : "cannot mount!");
        }
        g_list_free_full (volumes, g_object_unref);
    }

    g_list_free_full (drives, g_object_unref);
    g_object_unref (vm);

    return 0;
}
