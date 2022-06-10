/*************************************************************************
> FileName: gio-list-all-attribute.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月18日 星期五 13时37分49秒
 ************************************************************************/

#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[]) 
{
    if (argc < 2) {
        printf ("please input a path\n");
        return -1;
    }

    char**          attrs = NULL;
    GFile*          file = NULL;
    GError*         error = NULL;
    GFileInfo*      fileInfo = NULL;

    file = g_file_new_for_path (argv[1]);
    if (NULL == file) {
        printf ("get GFile error\n");
        goto end;
    }

    fileInfo = g_file_query_info (file, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, &error);
    if (NULL != error) {
        printf ("query error: %s\n", error->message);
        goto end;
    }

    if (NULL != fileInfo) {
        attrs = g_file_info_list_attributes (fileInfo, G_FILE_ATTRIBUTE_STANDARD_TYPE);
        printf ("\n----------------------------\n");
        for (int i = 0; attrs[i] != NULL; ++i) {
            printf ("attr-key: '%s'\n", attrs[i]);
        }
    } else {
        printf ("fileInfo is null\n");
    }

end:
    if (NULL != attrs) {
        g_strfreev (attrs);
    }

    if (NULL != error) {
        g_object_unref (error);
    }

    if (NULL != fileInfo) {
        g_object_unref (fileInfo);
    }

    if (NULL != file) {
        g_object_unref (file);
    }


}
