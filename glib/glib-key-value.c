/*************************************************************************
> FileName: glib-key-value.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 19 Dec 2023 11:35:49 AM CST
 ************************************************************************/
#include <glib.h>

int main (int argc, char* argv[])
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) key_file = g_key_file_new ();

    if (!g_key_file_load_from_file (key_file, "key-file.ini", G_KEY_FILE_KEEP_COMMENTS, &error)) {
        if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
            g_warning ("Error loading key file: %s", error->message);
        }
        return;
    }

    g_autofree gchar *val = g_key_file_get_string (key_file, "Group Name", "SomeKey", &error);
    if (val == NULL && !g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
        g_warning ("Error finding key in key file: %s", error->message);
        return;
    }
    else if (val == NULL) {
        // Fall back to a default value.
        val = g_strdup ("default-value");
    }

    return 0;
}
