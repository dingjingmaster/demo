/*************************************************************************
> FileName: gio-gmount.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年11月18日 星期三 15时38分02秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    if (argc <= 1) {
        printf ("请输入正确的uri!!!\n");
        return -1;
    }

    GFile* file = NULL;
    GMount* mount = NULL;
    GError* error = NULL;
    GFile* fileLocation = NULL;

    file = g_file_new_for_uri (argv[1]);
    if (NULL == file) {
        printf ("g_file_new_for_uri error!!!\n");
        goto end;
    }

    printf ("uri is exists: %s\n", g_file_query_exists(file, NULL)?"true":"false");

    mount = g_file_find_enclosing_mount(file, NULL, &error);
    if (NULL == mount || NULL != error) {
        printf ("g_file_find_enclosing_mount, error code: %d, error: %s\n", error->code, error->message);
        if (NULL != error) {
            g_object_unref (error);
            error = NULL;
        }
        goto end;
    }

    fileLocation = g_mount_get_default_location(mount);
    if (NULL == fileLocation) {
        printf ("g_mount_get_default_location, error\n");
        goto end;
    }

    printf ("\n=============================== 调用结果如下 ===============================================\n");
    printf ("g_mount_get_name: %30s\n", g_mount_get_name(mount));
    printf ("g_mount_get_uuid: %30s\n", g_mount_get_uuid(mount));
    printf ("g_mount_get_default_location uri: %30s\n", g_file_get_uri(fileLocation));
    printf ("\n============================================================================================\n");

end:
    if (NULL != file) {
        g_object_unref (file);
    }
    
    if (NULL != mount) {
        g_object_unref (mount);
    }

    if (NULL != error) {
        g_object_unref (error);
    }

    if (NULL != fileLocation) {
        g_object_unref (fileLocation);
    }

    return 0;
}

