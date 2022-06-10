/*************************************************************************
> FileName: gio-create.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月20日 星期六 11时35分54秒
 ************************************************************************/

#include <glib.h>
#include <fcntl.h>
#include <gio/gio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glib/gstdio.h>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要查询的文件路径!\n");
        return -1;
    }

    GError* error = NULL;

    GFile* file = g_file_new_for_path (argv[1]);
    if (NULL == file) {
        printf ("Gfile error!\n");
        return -1;
    }

    GFileInfo* fileinfo = g_file_query_info (file, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (NULL != error) {
        printf ("query file:%s's info error: %s\n", argv[1], error->message);
        g_error_free (error);
        goto end;
    }

    char* ctime = g_file_info_get_attribute_as_string(fileinfo, G_FILE_ATTRIBUTE_TIME_CREATED);

    printf ("The file '%s' create timestamp is %s\n", argv[1], ctime);

end:
    if (NULL != ctime) g_free (ctime);
    if (NULL != fileinfo) g_object_unref (fileinfo);
    if (NULL != file) g_object_unref (file);

    return 0;
}
