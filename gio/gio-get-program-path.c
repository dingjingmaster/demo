/*************************************************************************
> FileName: gio-get-program-path.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 14 Aug 2023 02:19:21 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <gio/gio.h>

bool get_program_full_path_to_array (char buf[], int bufLen);

int main (int argc, char* argv[])
{
    char buf[200] = {0};

    bool ret = get_program_full_path_to_array(buf, sizeof(buf)/sizeof(buf[0]));

    printf ("%s     :%s\n", (char*)buf, ret ? "true" : "false");

    return 0;
}

bool get_program_full_path_to_array (char buf[], int bufLen)
{
    if (!buf) return false;
    g_autoptr (GError) error = NULL;
    g_autofree char* cmdPath = g_strdup_printf ("/proc/%d/exe", getpid ());
    g_autofree char* programFullPath = g_file_read_link (cmdPath, &error);
    if (!programFullPath) {
        return false;
    }

    return (g_snprintf(buf, bufLen - 1, "%s", programFullPath) < bufLen);
}
