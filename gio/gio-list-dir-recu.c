/*************************************************************************
> FileName: gio-list-dir-recu.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 14 Aug 2023 10:15:53 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <gio/gio.h>

GList* get_file_recu (const char* file);
static bool file_is_dir (const GFile const * file);

int main (int argc, char* argv[])
{
    const char* rootPath = "/home/dingjing/aaa";

    GList* ls = get_file_recu (rootPath);
    if (!ls) {
        printf ("empty dir!\n");
        return 0;
    }

    printf ("file num: %d\n", g_list_length (ls));
    for (GList* l = g_list_first (ls); NULL != l; l = l->next) {
        printf ("  %s\n", (char*) l->data);
    }

    g_list_free_full (ls, g_free);

    return 0;
}

static bool file_is_dir (const GFile const * file)
{
    g_return_val_if_fail (G_IS_FILE(file), false);

    GFileType type = g_file_query_file_type (file, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL);

    if (G_FILE_TYPE_DIRECTORY != type) return false;

    return true;
}


GList* get_file_recu (const char* file)
{
    g_autoptr(GFile) f = g_file_new_for_path (file);
    if (!file_is_dir(f)) {
        GList* ls = NULL;
        if (g_file_query_exists (f, NULL)) {
            ls = g_list_append (ls, g_strdup(file));
        }
        return ls;
    }

    GList* dirs = NULL;
    GList* files = NULL;

    char* rootDir = g_strdup (file);

    dirs = g_list_append (dirs, rootDir);

    GList* ls = dirs;
    while (true) {
        if (NULL == ls) break;
        const char* path = (const char*) (ls->data);
        g_autoptr (GFile) f1 = g_file_new_for_path (path);
        if (!file_is_dir (f1)) {
            files = g_list_append (files, g_strdup(path));
            ls = g_list_next(ls);
            continue;
        }
        g_autoptr(GError) error = NULL;
        g_autoptr(GFileEnumerator) eum = g_file_enumerate_children (f1, "*", G_FILE_QUERY_INFO_NONE, NULL, &error);
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
                    if (0 == g_strcmp0(name, ".")) {
                        printf ("%s\n", name); 
                        continue;
                    }
                    g_autofree char* tpath = g_strdup_printf ("%s/%s", path, name);
                    if (G_FILE_TYPE_DIRECTORY == fileType) {
                        dirs = g_list_append (dirs, g_strdup(tpath));
                    }
                    else {
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

    g_list_free_full (dirs, g_free);

    return files;
}
