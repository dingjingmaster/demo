/*************************************************************************
> FileName: glib-gfile-path.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 02 Nov 2022 08:46:05 AM CST
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>
#include <syslog.h>

int main (int argc, char* argv[])
{
    g_autoptr(GFile) file1 = g_file_new_for_path("/media///dingjing/\xe4\xb8\x81\xe6\x95\xac/\xe6\x96\xb0\xe5\xbb\xba\xe6\x96\x87\xe4\xbb\xb6\xe5\xa4\xb9/dlp (副本)(副本).txt");
    g_autoptr(GFile) file2 = g_file_new_for_path("/media/dingjing/丁敬");

    g_autofree char* path = g_file_get_path(file1);

    printf ("%s\n%s\n", path, g_file_has_prefix(file1, file2) ? "true" : "false");
    syslog (LOG_ERR, "%s\n%s\n", path, g_file_has_prefix(file1, file2) ? "true" : "false");

    return 0;
}
