/*************************************************************************
> FileName: gio-gvfs.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年11月18日 星期三 17时52分05秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    GVfs* vfs = NULL;
    GFile* file = NULL;
    GFile* file1 = NULL;

    vfs = g_vfs_get_local();
    if (NULL == vfs) {
        printf ("vfs 錯誤!!!\n");
        goto end;
    }

    file = g_vfs_parse_name (vfs, "computer:///root.link");
    if (NULL == file) {
        printf ("g_vfs_parse_name error\n");
        goto end;
    }

    file1 = g_vfs_get_file_for_uri (vfs, "computer:///root.link");
    if (NULL == file1) {
        printf ("g_vfs_parse_name error\n");
        goto end;
    }

    printf ("uri: %s\n", g_file_get_uri(file));
    printf ("uri: %s\n", g_file_get_uri(file1));



end:
    if (NULL != vfs) {
        g_object_unref (vfs);
    }

    if (NULL != file) {
        g_object_unref (file);
    }

    if (NULL != file1) {
        g_object_unref (file1);
    }

    return 0;
}


