//
// Created by dingjing on 24-6-29.
//

#include "device-monitor.h"

#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>

struct _DeviceMonitor
{
    GHashTable*             devMount;
    GHashTable*             mountDev;
    GVolumeMonitor*         monitor;

    MountAdd                mountAdd;
    void*                   mountAddData;

    MountDel                mountDel;
    void*                   mountDelData;
};

static void _mount_add (DeviceMonitor* dm, const char* devName, const char* mountPoint);
static void _mount_del (DeviceMonitor* dm, const char* devName, const char* mountPoint);

static void mount_add (GVolumeMonitor* monitor, GMount* mount, gpointer data);
static void mount_del (GVolumeMonitor* monitor, GMount* mount, gpointer data);


DeviceMonitor *device_monitor_get(void)
{
    DeviceMonitor* dm = g_malloc0(sizeof(DeviceMonitor));
    g_assert(NULL != dm);

    dm->monitor = g_volume_monitor_get();
    g_assert(NULL != dm->monitor);

    dm->devMount = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    if (!dm->devMount) {
        device_monitor_destroy(&dm);
        return NULL;
    }

    dm->mountDev = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    if (!dm->mountDev) {
        device_monitor_destroy(&dm);
        return NULL;
    }

    dm->monitor = g_volume_monitor_get();
    if (!dm->monitor) {
        device_monitor_destroy(&dm);
        return NULL;
    }

    {
        g_signal_connect(G_OBJECT(dm->monitor), "mount-added", G_CALLBACK(mount_add), dm);
        g_signal_connect(G_OBJECT(dm->monitor), "mount-removed", G_CALLBACK(mount_del), dm);
    }

    return dm;
}

void device_monitor_destroy(DeviceMonitor **dm)
{
    g_return_if_fail(dm && *dm);

    if ((*dm)->monitor) {
        g_object_unref((*dm)->monitor);
    }

    if ((*dm)->mountDev) {
        g_hash_table_remove_all((*dm)->mountDev);
    }

    if ((*dm)->devMount) {
        g_hash_table_remove_all((*dm)->devMount);
    }

    g_free(*dm);
    *dm = NULL;
}

void device_monitor_set_mount_add(DeviceMonitor *dm, MountAdd add, void* udata)
{
    g_return_if_fail(dm && add);

    dm->mountAdd = add;
    dm->mountAddData = udata;
}

void device_monitor_set_mount_del(DeviceMonitor *dm, MountDel del, void* udata)
{
    g_return_if_fail(dm && del);

    dm->mountDel = del;
    dm->mountDelData = udata;
}

void device_monitor_get_current_mount_point(DeviceMonitor *dm)
{
    GList* ls = g_volume_monitor_get_mounts(dm->monitor);
    if (ls) {
        GList* itr = g_list_first(ls);
        for (; itr; itr = g_list_next(itr)) {
            char* rootDir = NULL;
            char* rootDev = NULL;
            GMount* m = (GMount*) (itr->data);
            if (m) {
                GFile* f = g_mount_get_root(m);
                if (f) {
                    rootDir = g_file_get_path(f);
                    g_object_unref(f);
                }

                GVolume* vol = g_mount_get_volume(m);
                if (vol) {
                    rootDev = g_volume_get_identifier(vol, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
                    g_object_unref(vol);
                }
            }

            _mount_add(dm, rootDev, rootDir);

            if (rootDir) {
                g_free(rootDir);
            }

            if (rootDev) {
                g_free(rootDev);
            }
        }
        g_list_free_full(ls, g_object_unref);
    }
}

static void mount_add (GVolumeMonitor* monitor, GMount* mount, gpointer data)
{
    g_return_if_fail(monitor && mount && data);

    DeviceMonitor* dm = (DeviceMonitor*) data;

    GFile* f = g_mount_get_root(mount);
    if (f) {
        char* rootDir = g_file_get_path(f);
        char* rootDev = NULL;

        GVolume* vol = g_mount_get_volume(mount);
        if (vol) {
            rootDev = g_volume_get_identifier(vol, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
            g_object_unref(vol);
        }

        if (rootDev && rootDir) {
            _mount_add(dm, rootDev, rootDir);
        }

        if (rootDev) { g_free(rootDev); }
        if (rootDir) { g_free(rootDir); }

        g_object_unref(f);
    }
}

static void mount_del (GVolumeMonitor* monitor, GMount* mount, gpointer data)
{
    g_return_if_fail(monitor && mount && data);

    DeviceMonitor* dm = (DeviceMonitor*) data;

    char* rootDir = NULL;
    char* rootDev = NULL;
    GFile* f = g_mount_get_default_location(mount);
    if (f) {
        rootDir = g_file_get_path(f);
        g_object_unref(f);
    }

    GVolume* v = g_mount_get_volume(mount);
    if (v) {
        rootDev = g_volume_get_identifier(v, G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE);
        g_object_unref(v);
    }

    _mount_del(dm, rootDev, rootDir);

    if (rootDir) { g_free(rootDir); }
    if (rootDev) { g_free(rootDev); }
}

static void _mount_add (DeviceMonitor* dm, const char* devName, const char* mountPoint)
{
    g_return_if_fail(dm && devName && mountPoint);

    if (!g_hash_table_contains(dm->devMount, devName)) {
        g_hash_table_insert(dm->devMount, g_strdup(devName), g_strdup(mountPoint));
    }

    if (!g_hash_table_contains(dm->mountDev, mountPoint)) {
        g_hash_table_insert(dm->mountDev, g_strdup(mountPoint), g_strdup(devName));

        if (dm->mountAdd) {
            (dm->mountAdd) (mountPoint, dm->mountAddData);
        }
    }
}

static void _mount_del (DeviceMonitor* dm, const char* devName, const char* mountPoint)
{
    g_return_if_fail(dm && (devName || mountPoint));

    if (mountPoint) {
        if (g_hash_table_contains(dm->mountDev, mountPoint)) {
            if (dm->mountDel) {
                (dm->mountDel) (mountPoint, dm->mountDelData);
            }

            const char* val = g_hash_table_lookup(dm->mountDev, mountPoint);
            if (val) {
                g_hash_table_remove(dm->mountDev, mountPoint);
            }
        }
    }

    if (devName) {
        if (g_hash_table_contains(dm->devMount, devName)) {
            const char* val = g_hash_table_lookup(dm->devMount, devName);
            if (val) {
                if (dm->mountDel) {
                    (dm->mountDel) (val, dm->mountDelData);
                }
                g_hash_table_remove(dm->devMount, devName);
            }
        }
    }
}
