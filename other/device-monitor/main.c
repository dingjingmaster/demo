#include <glib.h>
#include <stdio.h>

#include "device-monitor.h"

void mount_add (const char* mountPoint, void* udata);
void mount_del (const char* mountPoint, void* udata);

int main(void)
{
    DeviceMonitor* dm = device_monitor_get();
    if (!dm) {
        printf("get dm error!\n");
        return -1;
    }

    device_monitor_set_mount_add(dm, mount_add, dm);
    device_monitor_set_mount_del(dm, mount_del, dm);

    device_monitor_get_current_mount_point(dm);

    g_main_loop_run(g_main_loop_new(NULL, TRUE));

    device_monitor_destroy(&dm);

    return 0;
}

void mount_add (const char* mountPoint, void* udata)
{
    printf("新增挂载点： %s\n", mountPoint);
}

void mount_del (const char* mountPoint, void* udata)
{
    printf("移除挂载点：%s\n", mountPoint);
}