/*************************************************************************
> FileName: gio-volume-usb.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年08月05日 星期三 20时29分51秒
 ************************************************************************/

#include <stdio.h>
#include <gio/gio.h>

void mounted_func (gpointer data, gpointer udata)
{
    GMount* m = (GMount*)data;
    printf ("name:%s\n", g_mount_get_name(m));
    printf ("name:%s\n", g_mount_get_uuid(m));
    printf ("name:%s\n", g_icon_to_string(g_mount_get_icon(m)));
}

int main (int argc, char* argv[])
{
    GVolumeMonitor* vm = NULL;

    vm = g_volume_monitor_get();
    if (NULL == vm) {
        printf ("error\n");
        return -1;
    }

    GList* mounts = g_volume_monitor_get_mounts (vm);
    if (NULL == mounts) {
        printf ("no mount\n");
        return -1;
    }

    printf ("mount num:%d\n", g_list_length(mounts));
    printf ("================ mounted usb device ================\n");
    g_list_foreach (mounts, mounted_func, NULL);
    printf ("====================================================\n");

    return 0;
}
