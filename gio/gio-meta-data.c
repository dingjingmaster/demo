/*************************************************************************
> FileName: gio-meta-data.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月19日 星期五 09时14分36秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv)
{
    GError*         error = NULL;
    GFile*          file = g_file_new_for_path ("./a.o");
    GFile*          destFile = g_file_new_for_path ("./a.copy.o");

    g_file_create (file, G_FILE_CREATE_NONE, NULL, NULL);

    g_file_set_attribute (file, "metadata::meta-key:", G_FILE_ATTRIBUTE_TYPE_STRING, "meta-value", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (NULL != error) {
        printf ("set meta error: %s", error->message);
        g_error_free (error);
        return -1;
    }

    // copy
    g_file_copy (file, destFile, G_FILE_COPY_ALL_METADATA, NULL, NULL, NULL, &error);
    if (NULL != error) {
        printf ("copy error: %s", error->message);
        g_error_free (error);
        return -1;
    }

    if (NULL != file) g_object_unref (file);
    if (NULL != destFile) g_object_unref (destFile);

    return 0;
}

