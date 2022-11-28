/*************************************************************************
> FileName: gio-path-decode.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 28 Nov 2022 10:23:05 AM CST
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    const char* path1 = "file:///run/user/1000/gvfs/nfs:host=192.168.25.175,prefix=%2Fdata%2Fcode";
    const char* path2 = "nfs://192.168.25.175/data/code";
    const char* path3 = "/run/user/1000/gvfs/nfs:host=192.168.25.175,prefix=/data/code";
    const char* path4 = "/run/user/1000/gvfs/nfs:host=192.168.25.175,prefix=%2Fdata%2Fcode";

    g_autoptr(GFile) file1 = g_file_new_for_uri(path1);
    g_autoptr(GFile) file2 = g_file_new_for_uri(path2);
    g_autoptr(GFile) file3 = g_file_new_for_path(path3);
    g_autoptr(GFile) file4 = g_file_new_for_path(path4);

    printf ("path1\nuri: %s -- path: %s -- exists: %s\n", g_file_get_uri(file1), g_file_get_path(file1), g_file_query_exists(file1, NULL) ? "true" : "false");
    printf ("path2\nuri: %s -- path: %s -- exists: %s\n", g_file_get_uri(file2), g_file_get_path(file2), g_file_query_exists(file2, NULL) ? "true" : "false");
    printf ("path3\nuri: %s -- path: %s -- exists: %s\n", g_file_get_uri(file3), g_file_get_path(file3), g_file_query_exists(file3, NULL) ? "true" : "false");
    printf ("path4\nuri: %s -- path: %s -- exists: %s\n", g_file_get_uri(file4), g_file_get_path(file4), g_file_query_exists(file4, NULL) ? "true" : "false");

    return 0;
}
