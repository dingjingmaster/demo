/*************************************************************************
> FileName: gio-mount.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月14日 星期六 09时37分00秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int arg, char* garv[])
{
    GFile* upan = NULL;             // 相当于文件路径
    guint64 disk_usage = 0;
    guint64 num_dirs = 0;
    guint64 num_files = 0;
    GError* error = NULL;
    gboolean ret = FALSE;
    GMount* upan_mount = NULL;      // 已挂载的系统

    upan = g_file_new_for_path ("/data/tmp/mount_demo");

    // sdc
    printf ("basename:%s\n", g_file_get_basename(upan));

    printf ("parse_name:%s\n", g_file_get_parse_name(upan));
    
    // /dev/sdc
    printf ("path:%s\n", g_file_get_path(upan));

    // 1
    printf ("is native:%d\n", g_file_is_native(upan));

    // file
    printf ("uri scheme:%s\n", g_file_get_uri_scheme(upan));

    // 路径是不是存在
    printf ("query exists:%d\n", g_file_query_exists(upan, NULL));

    // 
    ret = g_file_measure_disk_usage (upan, G_FILE_MEASURE_APPARENT_SIZE, NULL, NULL, NULL, &disk_usage, &num_dirs, &num_files, &error);
    printf ("ret:%d --- %s\n", ret, ret==TRUE?"TRUE":"FALSE");
    if (FALSE == ret) {
        if (NULL != error->message)
            printf ("error: %s\n", error->message);
        g_error_free (error);
        goto end;
    }

    printf ("========\n");
    printf ("disk usage:%d --- %s\n", disk_usage, g_format_size(disk_usage));    // g_format_size 来自glib
    printf ("num files:%d, num_dirs:%d\n", num_files, num_dirs);
    printf ("========\n");
    
end:
    g_object_unref (upan);
    return 0;
}
