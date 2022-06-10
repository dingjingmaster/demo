/*************************************************************************
> FileName: gio-mount.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月16日 星期一 09时25分21秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

void mounts_func (gpointer data, gpointer udata)
{
    GMount* gm = (GMount*) data;

    /* 返回的字符串必须释放... */
    printf ("================== 开始 ======================\n");
    printf ("name:%s\n", g_mount_get_name(gm));
    printf ("uuid:%s\n", g_mount_get_uuid(gm));
    printf ("root:%s\n", g_file_get_path(g_mount_get_root(gm)));
    printf ("icon:%s\n", g_icon_to_string(g_mount_get_icon(gm)));
    printf ("================== 结束 ======================\n\n");
}

/* 获取挂载的卷 */
int main (int argc, char* argv[])
{
    GVolumeMonitor* gm = NULL;
    GList* mounts = NULL;

    gm = g_volume_monitor_get();
    mounts = g_volume_monitor_get_mounts (gm);
    if (NULL == mounts) {
        printf ("返回挂载点失败!\n");
        goto end;
    }
    g_list_foreach (mounts, mounts_func, NULL);
    g_list_free (mounts);

end:
    g_object_unref (gm);

    return 0;
}
