/*************************************************************************
> FileName: gio-volume-info.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年10月28日 星期四 16时02分11秒
 ************************************************************************/
#include <gio/gio.h>
#include <gio/gunixmounts.h>

static gboolean query_info (GFile* file);
static char* escape_string (const char* in);
static void show_attributes (GFileInfo* info);
static void show_info (GFile* file, GFileInfo* info);
static void handle_mount_changed (GVolumeMonitor* monitor, GMount* mount, gpointer data);

int main (int argc, char* argv[])
{
    GVolumeMonitor* monitor = g_volume_monitor_get();

    g_signal_connect(G_OBJECT(monitor), "mount-added", G_CALLBACK (handle_mount_changed), "mount-added");

    g_main_loop_run (g_main_loop_new(0,0));

    return 0;
}


static void handle_mount_changed (GVolumeMonitor* monitor, GMount* mount, gpointer data)
{
    g_usleep (800000);
    char* ev = (char*)data;
    if (g_strcmp0(ev, "mount-removed") == 0) {
        return;
    }

    g_return_if_fail (G_IS_MOUNT (mount));

    g_autoptr (GVolume) volume = g_mount_get_volume (mount);

    g_return_if_fail (G_IS_VOLUME (volume));

    g_autoptr (GFile) file = g_volume_get_activation_root (volume);

    if (G_IS_FILE (file)) {
        query_info (file);
    }
}

static gboolean query_info (GFile* file)
{
    g_return_val_if_fail (G_IS_FILE (file), FALSE);

    g_autoptr (GError) error = NULL;
    GFileQueryInfoFlags flags = G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS;

    GFileInfo* info = g_file_query_info (file, "*", flags, NULL, &error);
    if (info == NULL) {
        printf ("%d == %s\n", error->code, error->message);
        return FALSE;
    }

    show_info (file, info);

    return TRUE;
}

static void show_info (GFile* file, GFileInfo* info)
{
    const char *name, *type;
    char *escaped, *uri;
    goffset size;
    const char *path;
#ifdef G_OS_UNIX
    GUnixMountEntry *entry;
#endif

    name = g_file_info_get_display_name (info);
    if (name)
    /* Translators: This is a noun and represents and attribute of a file */
        g_print ("display name: %s\n", name);

    name = g_file_info_get_edit_name (info);
    if (name)
    /* Translators: This is a noun and represents and attribute of a file */
        g_print ("edit name: %s\n", name);

    name = g_file_info_get_name (info);
    if (name) {
        escaped = escape_string (name);
        g_print ("name: %s\n", escaped);
        g_free (escaped);
    }

    if (g_file_info_has_attribute (info, G_FILE_ATTRIBUTE_STANDARD_SIZE)) {
        size = g_file_info_get_size (info);
        g_print ("size: ");
        g_print (" %"G_GUINT64_FORMAT"\n", (guint64)size);
    }

    if (g_file_info_get_is_hidden (info))
        g_print ("hidden\n");

    uri = g_file_get_uri (file);
    g_print ("uri: %s\n", uri);
    g_free (uri);

    path = g_file_peek_path (file);
    if (path) {
        g_print ("local path: %s\n", path);

#ifdef G_OS_UNIX
        entry = g_unix_mount_at (path, NULL);
        if (entry == NULL)
            entry = g_unix_mount_for (path, NULL);
        if (entry != NULL) {
            gchar *device;
            const gchar *root;
            gchar *root_string = NULL;
            gchar *mount;
            gchar *fs;
            const gchar *options;
            gchar *options_string = NULL;

            device = g_strescape (g_unix_mount_get_device_path (entry), NULL);
            root = g_unix_mount_get_root_path (entry);
            if (root != NULL && g_strcmp0 (root, "/") != 0) {
                escaped = g_strescape (root, NULL);
                root_string = g_strconcat ("[", escaped, "]", NULL);
                g_free (escaped);
            }
            mount = g_strescape (g_unix_mount_get_mount_path (entry), NULL);
            fs = g_strescape (g_unix_mount_get_fs_type (entry), NULL);

            options = g_unix_mount_get_options (entry);
            if (options != NULL) {
              options_string = g_strescape (options, NULL);
            }

            g_print ("unix mount: %s%s %s %s %s\n", device,
                   root_string ? root_string : "", mount, fs,
                   options_string ? options_string : "");

            g_free (device);
            g_free (root_string);
            g_free (mount);
            g_free (fs);
            g_free (options_string);

            g_unix_mount_free (entry);
        }
#endif
    }

    show_attributes (info);
}

static void show_attributes (GFileInfo *info)
{
    char **attributes;
    char *s;
    int i;

    attributes = g_file_info_list_attributes (info, NULL);

    g_print ("attributes:\n");
    for (i = 0; attributes[i] != NULL; i++) {
        if (strcmp (attributes[i], "standard::icon") == 0 || strcmp (attributes[i], "standard::symbolic-icon") == 0) {
            GIcon *icon;
            int j;
            const char * const *names = NULL;

            if (strcmp (attributes[i], "standard::symbolic-icon") == 0)
                icon = g_file_info_get_symbolic_icon (info);
            else
                icon = g_file_info_get_icon (info);

            /* only look up names if GThemedIcon */
            if (G_IS_THEMED_ICON(icon)) {
                names = g_themed_icon_get_names (G_THEMED_ICON (icon));
                g_print ("  %s: ", attributes[i]);
                for (j = 0; names[j] != NULL; j++)
                    g_print ("%s%s", names[j], (names[j+1] == NULL)?"":", ");
                g_print ("\n");
            } else {
                s = g_file_info_get_attribute_as_string (info, attributes[i]);
                g_print ("  %s: %s\n", attributes[i], s);
                g_free (s);
            }
        } else {
            s = g_file_info_get_attribute_as_string (info, attributes[i]);
            g_print ("  %s: %s\n", attributes[i], s);
            g_free (s);
        }
    }
  
    g_strfreev (attributes);
}

static char* escape_string (const char *in)
{
    GString *str;
    static char *hex_digits = "0123456789abcdef";
    unsigned char c;

    str = g_string_new ("");

    while ((c = *in++) != 0) {
        if (c >= 32 && c <= 126 && c != '\\') {
            g_string_append_c (str, c);
        } else {
            g_string_append (str, "\\x");
            g_string_append_c (str, hex_digits[(c >> 4) & 0xf]);
            g_string_append_c (str, hex_digits[c & 0xf]);
        }
    }

    return g_string_free (str, FALSE);
}
