/*************************************************************************
> FileName: gio-meta-time.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月04日 星期四 13时47分02秒
 ************************************************************************/
#include <gio/gio.h>

void saveCreateTime(const char* url);
gint64 getCreateTimeOfMicro(const char* url);

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要修改的文件路径\n");
        return -1;
    }

    char* path = NULL;

    if (g_str_has_prefix (argv[1], "file://")) {
        path = g_strdup (argv[1]);
    } else if (g_str_has_prefix (argv[1], "/")) {
        path = g_strdup_printf ("file://%s", argv[1]);
    } else {
        path = g_strdup_printf ("file://%s/%s", g_get_current_dir(), argv[1]);
    }

    saveCreateTime (path);
    getCreateTimeOfMicro (path);

    if (path) g_free (path);

    return 0;
}

void saveCreateTime(const char* url)
{
    g_autoptr (GError) error = NULL;
    g_autoptr (GFile) file = g_file_new_for_uri (url);
    g_autofree gchar* currentTime = g_strdup_printf ("%ld", g_get_real_time ());

    printf ("===> currentTime:%s\n", currentTime);

    g_return_if_fail (G_IS_FILE (file) && g_file_query_exists (file, NULL));

    g_file_set_attribute (file, "metadata::ctime", G_FILE_ATTRIBUTE_TYPE_STRING, currentTime, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);

    printf ("===> set file: %s   to   %s\n", g_file_peek_path (file), currentTime);

    if (error)      printf ("set create time error: %s\n", error->message);
}

gint64 getCreateTimeOfMicro(const char* url)
{
    g_autoptr (GError) error = NULL;
    g_autoptr (GFile) file = g_file_new_for_uri (url);
    g_autoptr (GFileInfo) fileInfo = g_file_query_info (file, G_FILE_ATTRIBUTE_TIME_CHANGED "," G_FILE_ATTRIBUTE_TIME_CREATED "," "metadata::ctime", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);

    g_return_val_if_fail (G_IS_FILE (file) && G_IS_FILE_INFO (fileInfo) && g_file_query_exists (file, NULL), 0);

#if 0 
    printf ("list attributes: ========================>\n");
    char** attrs = g_file_info_list_attributes (fileInfo, "*");
    for (int i = 0; attrs != NULL && attrs[i] != NULL; ++i) {
        printf ("%s    ", attrs[i]);
    }
    printf ("==========================================\n");
#endif

#if 0
    if (g_file_info_has_attribute (fileInfo, G_FILE_ATTRIBUTE_TIME_CREATED)) {
        printf ("has attribute  -->  %s", G_FILE_ATTRIBUTE_TIME_CREATED);
        gint64 createTime = g_file_info_get_attribute_uint64 (fileInfo, G_FILE_ATTRIBUTE_TIME_CREATED);
        gint64 modifyTime = g_file_info_get_attribute_uint64 (fileInfo, G_FILE_ATTRIBUTE_TIME_CHANGED);
        if (createTime != 0 && createTime <= modifyTime) {
            printf ("%s ==> %d\n", __FUNCTION__, __LINE__);
            return createTime;
        }
    }
#endif 
#define debug1 0

#if debug1
    if (g_file_info_has_attribute (fileInfo, "metadata::ctime")) {
#endif
        printf ("has attribute  -->  metadata::ctime --> %d  --> %s\n", g_file_info_has_attribute (fileInfo, "metadata::ctime"), g_file_info_get_attribute_as_string (fileInfo, "metadata::ctime"));
        const gchar* createTimeStr = g_file_info_get_attribute_string (fileInfo, "metadata::ctime");
        if (createTimeStr) {
            g_autofree char* createTime10 = g_strndup (createTimeStr, 10);
            printf ("ctime:%s\n", createTime10);
            return atoll (createTime10);
        }
#if debug1
    }
#endif
    printf ("%s ==> %d\n", __FUNCTION__, __LINE__);

    return 0;
}
