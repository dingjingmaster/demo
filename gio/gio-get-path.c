/*************************************************************************
> FileName: get-path.c
> Author  : Jun
> Mail    : Jun
> Created Time: 2020年06月21日 星期日 10时52分14秒
 ************************************************************************/
#include <gio/gio.h>

int main ()
{
    const char* uri = "computer:///SanDisk SD9TB8W256G1001.drive";
    GFile * file = g_file_new_for_uri (uri);

    printf ("path: %s\n", g_file_get_path(file));

    return 0;
}

