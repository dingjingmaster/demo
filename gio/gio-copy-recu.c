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
static void copy_file (const char* srcBaseDir, const char* srcFile, const char* destDir);

int main (int argc, char* argv[])
{
    if (argc != 3) {
        printf("Please input src dir and dest dir.\n");
        return -1;
    }

    // fixme:// 
    const char* rootPath = argv[1];
    const char* destRoot = argv[2];

    GList* ls = get_file_recu (rootPath);
    if (!ls) {
        printf ("empty dir!\n");
        return 0;
    }

    for (GList* l = g_list_first (ls); NULL != l; l = l->next) {
        copy_file(rootPath, l->data, destRoot);
        printf ("  %s\n", (char*) l->data);
    }
    printf ("file num: %d\n", g_list_length (ls));

    g_list_free_full (ls, g_free);

    return 0;
}

static void copy_file (const char* srcBaseDir, const char* srcFile, const char* destDir)
{
#define FREE_STR(str)                   do { if (str) { g_free(str); str = NULL; } } while (0)
#define FREE_GOBJECT(obj)               do { if (obj) { g_object_unref (obj); obj = NULL; } } while (0)
    if (!srcBaseDir || !srcFile || !destDir) { return; }
    if (!g_str_has_prefix(srcFile, srcBaseDir)) { return; }

    char* strTmp = NULL;
    GFile* srcGFile = NULL;
    GFile* dstGFile = NULL;
    GFile* dstParentFile = NULL;
    char* dstFile = NULL;

    const char* found = strstr(srcFile, srcBaseDir);
    const char* subStr = srcFile + strlen(srcBaseDir);
    if (!subStr) { goto out; }

    dstFile = g_strdup_printf("%s%s", destDir, subStr);

    printf ("substr: %s --> dest: %s\n", subStr, dstFile);

    srcGFile = g_file_new_for_path(srcFile);
    dstGFile = g_file_new_for_path(dstFile);
    if (!G_IS_FILE(srcGFile) || !G_IS_FILE(dstGFile)) { goto out; }

    dstParentFile = g_file_get_parent(dstGFile);

    // 源文件是 文件夹，则在目标文件夹中创建文件夹即可
    if (file_is_dir(srcGFile)) {
        FREE_STR(strTmp);
        g_mkdir_with_parents(dstFile, 0755);
        goto out;
    }

    FREE_STR(strTmp);
    strTmp = g_file_get_path(dstParentFile);
    g_mkdir_with_parents(strTmp, 0755);
    g_file_copy (srcGFile, dstGFile, G_FILE_COPY_OVERWRITE | G_FILE_COPY_ALL_METADATA, NULL, NULL, NULL, NULL);

out:
    FREE_STR(strTmp);
    FREE_GOBJECT(srcGFile);
    FREE_GOBJECT(dstGFile);
    FREE_GOBJECT(dstParentFile);
    if (dstFile) { g_free (dstFile); dstFile = NULL; }
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
    if (!file) { return NULL; }

#define FREE_GOBJECT(obj)               do { if (obj) { g_object_unref (obj); obj = NULL; } } while (0)
#define FREE_LIST_STR_NODE(obj)         do { if (obj) { g_list_free_full (obj, g_free); obj = NULL; } } while (0)
#define GET_LIST_FIRST_NODE(list, node) do { if (node) { g_list_free_full (node, g_free); node = NULL; } if (list) { node = g_list_first(list); list = g_list_remove_link(list, node); } } while (0)

    char* rootDir = g_strdup (file);            // no free
    GList* dirs = NULL;                         // free         OK
    GList* files = NULL;                        // no free
    GError* error = NULL;                       // free
    GList* listNodeTmp = NULL;                  // free      
    GFile* fileTmp = NULL;                      // free         OK
    GFileEnumerator* fileEum = NULL;            // free         OK
    GFileInfo* fileInfoTmp = NULL;              // free         OK

    fileTmp = g_file_new_for_path (file);
    if (!file_is_dir(fileTmp)) {
        GList* ls = NULL;
        if (g_file_query_exists (fileTmp, NULL)) {
            ls = g_list_append (ls, g_strdup(file));
        }
        goto out;
    }

    dirs = g_list_append (dirs, rootDir);
    FREE_LIST_STR_NODE(listNodeTmp);
    GET_LIST_FIRST_NODE(dirs, listNodeTmp);
    while (true) {
        if (NULL == listNodeTmp) break;
        const char* path = (const char*) (listNodeTmp->data);
        FREE_GOBJECT(fileTmp);
        fileTmp = g_file_new_for_path (path);
        if (!file_is_dir (fileTmp)) {
            files = g_list_append (files, g_strdup(path));
            FREE_LIST_STR_NODE(listNodeTmp);
            GET_LIST_FIRST_NODE(dirs, listNodeTmp);
            continue;
        }

        FREE_GOBJECT(fileEum);
        fileEum = g_file_enumerate_children (fileTmp, "*", G_FILE_QUERY_INFO_NONE, NULL, &error);
        if (error) {
            printf ("error: %s\n", error->message);
            FREE_LIST_STR_NODE(listNodeTmp);
            GET_LIST_FIRST_NODE(dirs, listNodeTmp);
            continue;
        }

        if (G_IS_FILE_ENUMERATOR(fileEum)) {
            FREE_GOBJECT(fileInfoTmp);
            fileInfoTmp = g_file_enumerator_next_file (fileEum, NULL, &error);
            while (fileInfoTmp) {
                if (error) {
                    printf ("code: %d, msg: %s\n", error->code, error->message);
                    FREE_GOBJECT(fileInfoTmp);
                    fileInfoTmp = g_file_enumerator_next_file (fileEum, NULL, NULL);
                    continue;
                }

                const char* name = g_file_info_get_display_name (fileInfoTmp);
                int fileType = g_file_info_get_attribute_uint32 (fileInfoTmp, G_FILE_ATTRIBUTE_STANDARD_TYPE);
                if (name) {
                    if (0 == g_strcmp0(name, ".") || 0 == g_strcmp0(name, "..")) {
                        printf ("%s\n", name); 
                        continue;
                    }
                    char* tpath = g_strdup_printf ("%s/%s", path, name);
                    if (G_FILE_TYPE_DIRECTORY == fileType) {
                        dirs = g_list_append (dirs, tpath);
                        files = g_list_append (files, g_strdup(tpath));
                    }
                    else {
                        files = g_list_append (files, tpath);
                    }
                }
                FREE_GOBJECT(fileInfoTmp);
                fileInfoTmp = g_file_enumerator_next_file (fileEum, NULL, NULL);
            }
        } else {
            printf ("error 1\n");
        }
        FREE_LIST_STR_NODE(listNodeTmp);
        GET_LIST_FIRST_NODE(dirs, listNodeTmp);
    }

out:
    if (error)          { g_error_free (error); error = NULL; }
    if (fileTmp)        { FREE_GOBJECT (fileTmp); }
    if (fileEum)        { FREE_GOBJECT (fileEum); }
    if (fileInfoTmp)    { FREE_GOBJECT (fileInfoTmp); }
    if (dirs)           { g_list_free_full (dirs, g_free); dirs = NULL; }

    return files;
}
