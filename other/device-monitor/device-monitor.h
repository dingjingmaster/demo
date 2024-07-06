//
// Created by dingjing on 24-6-29.
//

#ifndef device_monitor_DEVICE_MONITOR_DEVICE_MONITOR_H
#define device_monitor_DEVICE_MONITOR_DEVICE_MONITOR_H

typedef struct _DeviceMonitor DeviceMonitor;

typedef void (*MountAdd) (const char*, void* udata);
typedef void (*MountDel) (const char*, void* udata);

DeviceMonitor* device_monitor_get (void);

void device_monitor_get_current_mount_point (DeviceMonitor* dm);

void device_monitor_destroy(DeviceMonitor** dm);

void device_monitor_set_mount_add (DeviceMonitor* dm, MountAdd add, void* udata);
void device_monitor_set_mount_del (DeviceMonitor* dm, MountDel del, void* udata);

#endif // device_monitor_DEVICE_MONITOR_DEVICE_MONITOR_H
