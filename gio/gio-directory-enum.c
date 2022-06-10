/*************************************************************************
> FileName: gio-directory-enum.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年06月16日 星期二 11时20分28秒
 ************************************************************************/

#include <glib.h>
#include <glib/gprintf.h>
#include <gio-unix-2.0/gio/gdesktopappinfo.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <glib/gstdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <gmodule.h>

#define INSERT_KEY_VALUE(a,b) g_hash_table_insert(name_discription_table, a, b)

typedef struct thread_data
{
    char * basename ;
    char* description;
} hash_element;

static GHashTable *name_discription_table = NULL;

static void name_discription_table_init ();

static void *change_description_func (void *data)
{
    hash_element *element = (hash_element*)data;
    char *basename = element->basename;
    char *source_path = g_strconcat ("/usr/share/applications/", basename, NULL);
    char *dest_path = g_strconcat (g_get_home_dir (), "/.local/share/applications/", basename, NULL);

    printf ("%s\n%s\n%s\n%s\n\n", basename, element->description, source_path, dest_path);
    
    GFile *source_file = g_file_new_for_path (source_path);
    GFile *dest_file = g_file_new_for_path (dest_path);

    if (!g_file_test (dest_path, G_FILE_TEST_EXISTS)) {
        printf ("no file exist\n");
        //copy and change the value of name[zh-CN]
        gboolean success = g_file_copy (source_file, dest_file, /*G_FILE_COPY_OVERWRITE*/ G_FILE_COPY_NONE, NULL, NULL, NULL, NULL);
        if (success) {
            printf ("success\n");
            GKeyFile *key_file = g_key_file_new ();
            gboolean is_loaded = g_key_file_load_from_file (key_file, dest_path, G_KEY_FILE_KEEP_TRANSLATIONS, NULL);
            if (is_loaded) {
                char *name = g_key_file_get_value (key_file, G_KEY_FILE_DESKTOP_GROUP, "Name[zh_CN]", NULL);
                printf ("name zh_CN:%s\n", name);
                char *new_name = element->description;
                printf ("%s\n", new_name);
                g_key_file_set_value (key_file, G_KEY_FILE_DESKTOP_GROUP, "Name[zh_CN]", new_name);
                gboolean saved = g_key_file_save_to_file (key_file, dest_path, NULL);
                if (!saved) {
                    printf ("saved failed!\n");
                }
                printf ("%d\n",g_chmod (dest_path, 493)); //八进制775的十进制转换
            }
            g_key_file_free (key_file); 
        }
    } else {
        /*
        //check if name[zh_CN] is same with element->description
        //should it be done?
        GKeyFile *key_file = g_key_file_new ();
        gboolean is_loaded = g_key_file_load_from_file (key_file, dest_path, G_KEY_FILE_KEEP_TRANSLATIONS, NULL);
        if (is_loaded) {
            char *name = g_key_file_get_value (key_file, G_KEY_FILE_DESKTOP_GROUP, "Name[zh_CN]", NULL);
            printf ("name zh_CN:%s\n", name);
            char *new_name = element->description;
            printf ("%s\n", new_name);
            gboolean is_description_same = g_str_equal (name, new_name);
            if (!is_description_same) {
                g_key_file_set_value (key_file, G_KEY_FILE_DESKTOP_GROUP, "Name[zh_CN]", new_name);
                gboolean saved = g_key_file_save_to_file (key_file, dest_path, NULL);
                if (!saved) {
                    printf ("saved failed!\n");
                }
                printf ("%d\n",g_chmod (dest_path, 493)); //八进制775的十进制转换
            }
        }
        g_key_file_free (key_file); 
        */
    }
    g_object_unref (source_file);
    g_object_unref (dest_file);

    g_free (source_path);
    g_free (dest_path);

    g_free (element->basename);
    g_free (element->description);
    g_free (element);
    printf ("thread done\n");
}

static gboolean equal_func (gconstpointer a, gconstpointer b)
{
    return g_str_equal (a, b) ? TRUE :  FALSE;
}

static void file_added_callback (GFileMonitor* monitor, GFile *file, GFile *other_file, GFileMonitorEvent event_type, gpointer user_data)
{

    const char* name = g_file_get_path (file);
    const char* basename = g_file_get_basename (file);
    char* dest_path = g_strconcat (g_get_home_dir (), "/.local/share/applications/", basename, NULL);
    GFile *dest_file = g_file_new_for_path (dest_path);
    switch (event_type) {
        case G_FILE_MONITOR_EVENT_MOVED_IN:
            printf ("G_FILE_MONITOR_EVENT_MOVED_IN %s\n", name);
            break;

        case G_FILE_MONITOR_EVENT_CREATED:
            printf ("G_FILE_MONITOR_EVENT_CREATED %s\n", name);
            
            printf ("basename: %s\n", basename);
            const char* description_in_table = g_hash_table_lookup (name_discription_table, basename);
            if (description_in_table) {
                printf ("callback: find a discription at hash: %s\n\n\n\n\n", description_in_table);
                printf ("dest_path: %s\n", dest_path);

                if (!g_file_test (dest_path, G_FILE_TEST_EXISTS)) {
                    printf ("prepare for thread func\n");
                    hash_element *element = g_new0 (hash_element, 1);
                    element->basename = g_strdup (basename);
                    element->description = g_strdup (description_in_table);
                    pthread_t t1;
                    pthread_create (&t1, NULL, change_description_func, element);
                }
            }

            g_free (dest_path);
            g_object_unref (dest_file);
            break;
                   
        case G_FILE_MONITOR_EVENT_DELETED:
            //delete desktop file in .local/share/applications/
            printf ("G_FILE_MONITOR_EVENT_DELETED\n");
            g_file_delete (dest_file, NULL, NULL);

            g_free (dest_path);
            g_object_unref (dest_file);
            break;
            
        default:
            break;
    }
}

int main()
{
    int i = 0;
    GFileInfo *info = NULL;

    name_discription_table_init ();
    //gtk_init (NULL, NULL);
    
    GFile *location = g_file_new_for_path ("/usr/share/applications");

    printf ("开始遍历===================\n");
    GFileEnumerator *enumerator = g_file_enumerate_children (location, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NONE, NULL, NULL);
    info = g_file_enumerator_next_file (enumerator, NULL, NULL);
    while (info) {
        i++;
        const char* name = g_file_info_get_display_name (info);
        const char* tmp = g_hash_table_lookup (name_discription_table, name);
        printf ("%d: %s\n", i, name);
        if (tmp) {
            printf ("find a discription at hash: %s\n\n\n\n\n", tmp);

            char *dest_path = g_strconcat (g_get_home_dir (), "/.local/share/applications/", name, NULL);

            // 家目录下存在则比较 描述信息
            if (!g_file_test (dest_path, G_FILE_TEST_EXISTS)) {
                printf ("prepare for thread func\n");
                hash_element *element = g_new0 (hash_element, 1);
                element->basename = g_strdup (name);
                element->description = g_strdup (tmp);
                pthread_t t1;
                pthread_create (&t1, NULL, change_description_func, element);
            }
            g_free (dest_path);
        }
        g_object_unref (info);
        info = g_file_enumerator_next_file (enumerator, NULL, NULL);
    }
    g_object_unref (enumerator);
    printf ("遍历完成===================\n");

    printf ("home dir: %s\n", g_get_home_dir ());
    printf ("user data dir: %s\n", g_get_user_data_dir ());

    //GFileMonitor *monitor = g_file_monitor_directory (location, G_FILE_MONITOR_EVENT_CREATED, NULL, NULL);
    //g_signal_connect (monitor, "changed", G_CALLBACK (file_added_callback), NULL);

    g_object_unref (location);

    //gtk_main ();
    if (name_discription_table) {
        g_hash_table_destroy (name_discription_table);
    }
    return 0;
}

void name_discription_table_init ()
{
    if (name_discription_table) {
        g_hash_table_destroy (name_discription_table);
    }
    name_discription_table = g_hash_table_new (g_str_hash, equal_func);
    //add key-value here;
    //g_hash_table_insert (name_discription_table, "firefox.desktop", "浏览器");
    //g_hash_table_insert (name_discription_table, "pluma.desktop", "记事本");
    //g_hash_table_insert (name_discription_table, "mate-terminal.desktop", "终端命令行");
    INSERT_KEY_VALUE ("firefox.desktop","浏览器");
    INSERT_KEY_VALUE ("pluma.desktop","记事本");
    INSERT_KEY_VALUE ("mate-terminal.desktop", "终端命令行");
}
