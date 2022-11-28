/*************************************************************************
> FileName: gio-list-dir-files.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 28 Nov 2022 11:07:25 AM CST
 ************************************************************************/
#include <gio/gio.h>

#include <stdio.h>
#include <stdbool.h>

static bool is_dir (const GFile const* file) 
{
    g_return_val_if_fail (G_IS_FILE(file), false);

    GFileType type = g_file_query_file_type (file, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL);

    if (type != G_FILE_TYPE_DIRECTORY)  return false;

    return true;
}

int main (int argc, char* argv[])
{
    GList* dirs = NULL;
    GList* files = NULL;

    char* rootDir = g_strdup("/home/dingjing/");        // 要遍历的根目录

    dirs = g_list_append (dirs, rootDir);

    GList* ls = dirs;
    while (true) {
        if (NULL == ls) break;
        const char* path = (const char*)(ls->data);
        g_autoptr(GFile) file = g_file_new_for_path (path);
        printf ("-->%s\n", path);
        if (!is_dir (file)) {
            files = g_list_append (files, g_strdup(path));
            ls = g_list_next(ls);
            continue;
        }
        g_autoptr(GError) error = NULL;
        g_autoptr(GFileEnumerator) eum = g_file_enumerate_children (file, "*", G_FILE_QUERY_INFO_NONE, NULL, &error);
        if (error) {
            printf ("error: %s\n", error->message);
            ls = g_list_next(ls);
            continue;
        }

        if (G_IS_FILE_ENUMERATOR(eum)) {
            g_autoptr(GFileInfo) fileInfo = g_file_enumerator_next_file (eum, NULL, &error);
            while (fileInfo) {
                g_autoptr(GError) error = NULL;
                if (error) {
                    printf ("code: %d, msg: %s\n", error->code, error->message);
                    continue;
                }
                if (NULL == fileInfo) {
                    printf ("error 2\n");
                    break;
                }

                const char* name = g_file_info_get_display_name (fileInfo);
                int fileType = g_file_info_get_attribute_uint32 (fileInfo, G_FILE_ATTRIBUTE_STANDARD_TYPE);
                if (name) {
                    if (0 == g_strcmp0(name, ".")) {printf ("%s\n", name); return 1;}
                    g_autofree char* tpath = g_strdup_printf ("%s/%s", path, name);
                    if (G_FILE_TYPE_DIRECTORY == fileType) {
                        dirs = g_list_append (dirs, g_strdup(tpath));
                    } else {
                        files = g_list_append (files, g_strdup(tpath));
                    }
                }
                fileInfo = g_file_enumerator_next_file (eum, NULL, NULL);
            }
        } else {
            printf ("error 1\n");
        }

        ls = g_list_next(ls);
    }

    for (GList* f = dirs; NULL != f; f = g_list_next(f)) {
        printf ("[DIR]: %s\n", (char*) f->data);
    }


    for (GList* f = files; NULL != f; f = g_list_next(f)) {
        printf ("[FILE]: %s\n", (char*) f->data);
    }

    return 0;
}
