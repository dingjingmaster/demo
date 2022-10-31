/*************************************************************************
> FileName: parse-file-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 25 Oct 2022 04:02:20 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>
#include <stdbool.h>

const char* get_file_type (GFileType tp)
{
    switch (tp) {
        case G_FILE_TYPE_UNKNOWN:
            return "unknown";
        case G_FILE_TYPE_REGULAR:
            return "regular";
        case G_FILE_TYPE_DIRECTORY:
            return "directory";
        case G_FILE_TYPE_SYMBOLIC_LINK:
            return "symbolic link";
        case G_FILE_TYPE_SPECIAL:
            return "special";
        case G_FILE_TYPE_SHORTCUT:
            return "shortcut";
        case G_FILE_TYPE_MOUNTABLE:
            return "mountable";
    }
    return NULL;
}

bool is_symbolic_link (const char* file)
{
    g_autofree char* path = g_canonicalize_filename(file, NULL);
    if (path) {
        g_autoptr(GFile) file1 = g_file_new_for_path(path);
        if (G_IS_FILE(file1)) {
            GFileType fileType = g_file_query_file_type (file1, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL);
            return fileType == G_FILE_TYPE_SYMBOLIC_LINK;
        }
    }

    return false;
}

/**
 * 根据输入文件名获取 文件夹 和 文件名操作
 */
int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要查询的路径\n");
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
    }

    return 0;
}
