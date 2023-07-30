#include "favorites-vfs-file.h"

#include <QtCore>
#include <stdio.h>
#include <QString>

static GFile* test_vfs_parse_name (GVfs* vfs, const char* parse_name, gpointer user_data)
{
    QString tmp = parse_name;
    if (tmp.contains("real-uri:")) {
        QString realUri = tmp.split("real-uri:").last();
        return g_file_new_for_uri(realUri.toUtf8().constData());
    }

    return vfs_favorites_file_new_for_uri(parse_name);
}

static GFile* test_vfs_lookup (GVfs* vfs, const char* uri, gpointer user_data)
{
    return test_vfs_parse_name(vfs, uri, user_data);
}

int main(int argc, char *argv[])
{
    gint files = 0;
    GFile* fp = NULL;
    GFile* foc = NULL;
    GFileInfo* fi = NULL;
    GError* error = NULL;
    char** fileattr = NULL;
    GFileEnumerator* fs = NULL;
    GVfs* vfs = g_vfs_get_default();

    const gchar* const* schemas = g_vfs_get_supported_uri_schemes(vfs);
    const gchar* const* ps = schemas;

    while (*ps) {
        printf("schemas: %s\n", *ps);
        ++ps;
    }

    printf("=======================================\n\n");

#if GLIB_CHECK_VERSION(2, 50, 0)
    gboolean res = g_vfs_register_uri_scheme(vfs, "favorite", test_vfs_lookup, NULL, NULL, test_vfs_parse_name, NULL, NULL);
#endif

    schemas = g_vfs_get_supported_uri_schemes(vfs);
    ps = schemas;

    while (*ps) {
        printf("schemas: %s\n", *ps);
        ++ps;
    }


    // test
    printf ("\n1\n");
    GFile* f = g_file_new_for_uri("favorite:///");
    if (NULL == f) {
        printf ("g_file_new_for_uri error\n");
        return -1;
    }

    printf ("\n2\n");
    fs = g_file_enumerate_children (f, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);
    if (NULL == fs) {
        if (NULL != error) {
            printf ("error:%s", error->message);
        }
        g_error_free (error);
        g_object_unref (error);
        g_object_unref (f);
        return -1;
    }

    printf ("\n======================================================================\n");

    do {
        printf ("\n3\n");
        g_file_enumerator_iterate (fs, &fi, &foc, NULL, &error);
        printf ("\n3 - 1\n");
        if (NULL != error) {
            printf ("error:%s\n", error->message);
            g_error_free(error);
            g_object_unref (error);
            continue;
        }
        printf ("\n4\n");

        if (NULL == fi || NULL == foc) {
            printf ("stop enumerat! total:%d\n", files);
            g_file_enumerator_close (fs, NULL, NULL);
            break;
        }
        printf ("\n5\n");

        files += 1;
        printf ("============================= 开始 ===============================\n");
        printf ("path:%s\n", g_file_get_path(foc));
        printf ("uri:%s\n", g_file_get_uri(foc));
        printf ("name:%s\n", g_file_get_parse_name(foc));
        printf ("file type:%s\n", g_file_get_parse_name(foc));
        fileattr = g_file_info_list_attributes (fi, NULL);
        if (NULL != fileattr) {
            for (int i = 0; fileattr[i] != NULL; ++i) {
                printf ("%s ------ %s\n", fileattr[i], g_file_info_get_attribute_as_string(fi, fileattr[i]));
            }
        }
        printf ("============================= 结束 ===============================\n");
    } while (1);



}
