/*************************************************************************
> FileName: gvfs-chema.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 20 May 2022 09:23:06 AM CST
 ************************************************************************/
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    GVfs* vfs = g_vfs_get_default();

    const gchar* const* ss = g_vfs_get_supported_uri_schemes (vfs);

    for (int i = 0; ss[i]; ++i) {
        printf ("%s\n", ss[i]);
    }

    return 0;
}
