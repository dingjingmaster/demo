/*************************************************************************
> FileName: glib-is-symbolic-link.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Oct 2022 03:15:14 PM CST
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
        //printf ("path: %s\n", path);
        g_autoptr(GFile) file1 = g_file_new_for_path(path);
        if (G_IS_FILE(file1)) {
            GFileType fileType = g_file_query_file_type (file1, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL);
            //printf ("fileType: %s\n", get_file_type(fileType) ? get_file_type(fileType) : "null");
            return fileType == G_FILE_TYPE_SYMBOLIC_LINK;
        }
    }

    return false;
}

// 只有一层软连接的情况
char* get_symbolic_link (const char* path)
{
    g_autofree char* path1 = g_canonicalize_filename(path, NULL);
    if (is_symbolic_link(path)) {
        g_autoptr(GFile) file = g_file_new_for_path (path1);
        if (G_IS_FILE(file)) {
            g_autoptr(GFileInfo) fileInfo = g_file_query_info (file, G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET, G_FILE_QUERY_INFO_NONE, NULL, NULL);
            g_autofree char* symbolicLink = g_file_info_get_attribute_as_string (fileInfo, G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET);
            g_autoptr(GFile) parent = g_file_get_parent(file);
            if (G_IS_FILE(parent)) {
                g_autofree char* parentPath = g_file_get_path(parent);
                if (parentPath) {
                    g_autofree char* realSymbolicLink = g_canonicalize_filename(symbolicLink, parentPath);
                    printf ("path:%s\n", symbolicLink);
                    printf ("path:%s\n", realSymbolicLink);
                }
            }
        }
    }

    return NULL;
}

// 多层软连接的情况
char* get_symbolic_link2 (const char* path)
{
    // 最多处理十次软连接的情况
    int linkNum = 10;
    g_autofree char* dirTmp = NULL;  
    g_autofree char* pathTmp = g_strdup(path);

    if (!g_str_has_prefix(pathTmp, "/")) {
        dirTmp = g_strdup(g_get_current_dir());
    }

    while ((--linkNum >= 0) && is_symbolic_link(pathTmp)) {
        g_autofree char* path1 = g_canonicalize_filename(pathTmp, dirTmp);
        g_autoptr(GFile) file = g_file_new_for_path (path1);
        if (G_IS_FILE(file)) {
            g_autoptr(GFileInfo) fileInfo = g_file_query_info (file, G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET, G_FILE_QUERY_INFO_NONE, NULL, NULL);
            g_autofree char* symbolicLink = g_file_info_get_attribute_as_string (fileInfo, G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET);
            if (!g_str_has_prefix(symbolicLink, "/")) {
                g_autoptr(GFile) parent = g_file_get_parent(file);
                if (G_IS_FILE(parent)) {
                    g_autofree char* parentPath = g_file_get_path(parent);
                    if (parentPath) {
                        dirTmp = g_strdup(parentPath);
                        g_autofree char* realSymbolicLink = g_canonicalize_filename(symbolicLink, parentPath);
                        //printf ("p:%s, pp:%s\n", realSymbolicLink, parentPath);
                        if (!is_symbolic_link(realSymbolicLink)) {
                            return g_strdup(realSymbolicLink);
                        } else {
                            pathTmp = g_strdup(realSymbolicLink);
                        }
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            } else {
                if (!is_symbolic_link(symbolicLink)) {
                    return g_strdup(symbolicLink);
                } else {
                    dirTmp = NULL;
                    pathTmp = g_canonicalize_filename(symbolicLink, NULL);
                }
            }
        } else {
            break;
        }
    }

    return NULL;
}




int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要查询的路径\n");
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        //printf("%s\n", is_symbolic_link(argv[i]) ? "true" : "false");
        g_autofree char* pp = get_symbolic_link2(argv[i]);
        printf ("%s\n", pp ? pp : "null");
    }

    return 0;
}
