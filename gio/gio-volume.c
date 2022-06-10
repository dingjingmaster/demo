/*************************************************************************
> FileName: gio-volume.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月14日 星期六 13时12分46秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

/* 获取挂载卷和磁盘 */
void volume_func (gpointer data, gpointer udata)
{
    GVolume* v = (GVolume*)data;
    printf ("name:%s\n", g_volume_get_name(v));
    printf ("uuid:%s\n", g_volume_get_uuid(v));
}

// 遍历连接到电脑的驱动器
void driver_func (gpointer data, gpointer udata)
{
    GDrive* v = (GDrive*) data;
    GList* v1 = NULL;

    printf ("drive name:%s\n", g_drive_get_name(v));
    printf ("drive monted other volumes:%d\n", g_drive_has_volumes(v));
    printf ("drive can eject:%d\n", g_drive_can_eject(v));
    printf ("drive can stop:%d\n", g_drive_can_stop(v));
    // 低版本不支持
    //printf ("drive is removable:%d\n", g_drive_is_removable(v));
    printf ("drive can start degraded:%d\n", g_drive_can_start_degraded(v));

    v1 = g_drive_get_volumes(v);
    printf ("volume num:%d\n", g_list_length(v1));
    g_list_foreach (v1, volume_func, NULL);
    g_object_unref(v);
    printf ("\n\n");
}

void mounted_func (gpointer data, gpointer udata)
{
    GMount* m = (GMount*)data;
    printf ("name:%s\n", g_mount_get_name(m));
}

// 磁盘/卷 监控
int main (int argc, char* argv[])
{
    GVolumeMonitor* vm = NULL;
    GList* drives = NULL;
    GList* mounts = NULL;
    GList* volumes = NULL;

    vm = g_volume_monitor_get();
    if (NULL == vm) {
        printf ("错误!\n");
        return -1;
    }

#if 1
    printf ("drives 相关\n");
    drives = g_volume_monitor_get_connected_drives (vm);
    if (NULL == drives) {
        printf ("返回错误");
        return -1;
    }
    printf ("drives num:%d\n", g_list_length(drives));
    printf ("=========== 已连接的卷 ============\n");
    g_list_foreach (drives, driver_func, NULL);
    printf ("===================================\n");
#endif

#if 1
    printf ("volumes 相关\n");
    volumes = g_volume_monitor_get_volumes(vm);
    if (NULL == volumes) {
        printf ("no volume\n");
        return -1;
    }
    printf ("volumes num:%d\n", g_list_length(volumes));
#endif

#if 1
    printf ("mounts 相关\n");
    mounts = g_volume_monitor_get_mounts(vm);
    if (NULL == mounts) {
        printf ("no mount\n");
        return -1;
    }
    printf ("mounts num:%d\n", g_list_length(mounts));
    printf ("=========== mounted device ============\n");
    g_list_foreach (mounts, mounted_func, NULL);
    printf ("=======================================\n");
#endif

    // 元素释放
    //g_list_free (drives);
    g_object_unref (vm);

    return 0;
}
