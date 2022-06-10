/*************************************************************************
> FileName: gio-file-enumerate.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月16日 星期一 09时49分02秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv[])
{
    GFile* fp = NULL;
    GFile* foc = NULL;
    GFileInfo* fi = NULL;
    GFileEnumerator* fs = NULL;
    GError* error = NULL;
    gint files = 0;
    char** fileattr = NULL;

    fp = g_file_new_for_path ("/");
    fs = g_file_enumerate_children (fp, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (NULL == fs) {
        if (NULL != error) printf ("error:%s", error->message);
        g_error_free (error);
        g_object_unref (error);
        goto end;
    }
    do {
        // 返回值表示 "错误" 或 "正在迭代"
        g_file_enumerator_iterate (fs, &fi, &foc, NULL, error);
        if (NULL != error) {
            printf ("error:%s\n", error->message);
            g_error_free(error);
            g_object_unref (error);
            continue;
        }

        if (NULL == fi || NULL == foc) {
            printf ("结束迭代!文件总数:%d\n", files);
            g_file_enumerator_close (fs, NULL, NULL);
            break;
        }
        
        files += 1;
        printf ("============================= 开始 ===============================\n");
        printf ("path:%s\n", g_file_get_path(foc));
        printf ("uri:%s\n", g_file_get_uri(foc));
        printf ("name:%s\n", g_file_get_parse_name(foc));
        printf ("file type:%s\n", g_file_get_parse_name(foc));
        fileattr = g_file_info_list_attributes (fi, NULL);
        if (NULL != fileattr) {
            for (int i = 0; fileattr[i] != NULL; ++i) {
                printf ("%s ------ %s\n", fileattr[i], g_file_info_get_attribute_as_string(fi, fileattr[i]));
            }
        }
        printf ("============================= 结束 ===============================\n");
    } while (1);


end:
    if (NULL != fi) g_object_unref (fi);
    if (NULL != foc) g_object_unref (foc);
    if (NULL != fp) g_object_unref (fp);
    if (NULL != fs) g_object_unref (fs);
    return 0;
}
