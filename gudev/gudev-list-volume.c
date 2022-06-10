/*************************************************************************
> FileName: gudev-list-volume.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 15 Apr 2022 10:30:00 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gudev/gudev.h>

static void on_uevent (GUdevClient* client, const char* action, GUdevDevice* device, gpointer udata);

int main (int argc, char* argv[])
{
    const gchar* subsystems[] = {"block", "iscsi_connection", "scsi", NULL};

    g_autoptr (GUdevClient) uclient = g_udev_client_new (subsystems);
    g_return_val_if_fail (uclient, -1);

    g_signal_connect (uclient, "uevent", G_CALLBACK(on_uevent), NULL);

    g_main_loop_run (g_main_loop_new (NULL, TRUE));

    return 0;
}

static void on_uevent (GUdevClient* client, const char* action, GUdevDevice* device, gpointer udata)
{
    g_return_if_fail (client && action && device);

    const char* devName = g_udev_device_get_name (device);
    const char* devNumber = g_udev_device_get_number (device);
    const char* devSubsystem = g_udev_device_get_subsystem (device);
    const char* devType = g_udev_device_get_devtype (device);
    const char* devDriver = g_udev_device_get_driver (device);
    const char* devAction = g_udev_device_get_action (device);
    const char* devSysfsPath = g_udev_device_get_sysfs_path (device);

    guint64 devSeqnum = g_udev_device_get_seqnum (device);
    const char* devFile = g_udev_device_get_device_file (device);
    const char* const* devFileSymlinks = g_udev_device_get_device_file_symlinks (device);
    const char* const* propKeys = g_udev_device_get_property_keys (device);

    printf ("\n"
            "action: %s\n"
            "name: %s\n"
            "number: %s\n"
            "subsystem: %s\n"
            "type: %s\n"
            "driver: %s\n"
            "device action: %s\n"
            "sysfs path: %s\n"
            "device file: %s\n"
            "",
            action, devName, devNumber, devSubsystem, devType, devDriver, devAction, devSysfsPath, devFile);
    printf ("device file symlinks: ");
    for (int i = 0; devFileSymlinks[i] != NULL; ++i) {
        printf ("%s  ", devFileSymlinks[i]);
    }
    printf ("\n");

    printf ("property keys: ");
    for (int i = 0; propKeys[i] != NULL; ++i) {
        printf ("%s  ", propKeys[i]);
    }
    printf ("\n");

    printf ("tags: ");
    const gchar* const* tags = g_udev_device_get_tags (device);
    for (int i = 0; tags[i] != NULL; ++i) {
        printf ("%s  ", tags[i]);
    }
    printf ("\n");

    printf ("some prperties:\n");
    for (int i = 0; propKeys[i] != NULL; ++i) {
        if (g_udev_device_has_property (device, propKeys[i])) {
            printf ("%s: %s\n", propKeys[i], g_udev_device_get_property (device, propKeys[i]));
        }
    }
}
