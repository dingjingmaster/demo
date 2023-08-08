/*************************************************************************
> FileName: gdbus-call-dbus.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 04 Aug 2023 05:28:20 PM CST
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>
#include <glib/gprintf.h>

char** call_dbus_get_string_list (const char* name, const char* addr, const char* interface, const char* methodName);
void** call_dbus_get_object_list (const char* name, const char* addr, const char* interface, const char* methodName);
void   call_dbus_no_return       (const char* name, const char* addr, const char* interface, const char* methodName);

int main (int argc, char* argv[])
{
    char** arr = call_dbus_get_object_list ("com.deepin.filemanager.filedialog_x11", 
                "/com/deepin/filemanager/filedialogmanager_x11",
                "com.deepin.filemanager.filedialogmanager",
                "dialogs");

    // get paths;
    for (int i = 0; arr[i]; ++i) {
        printf (">>%s\n", arr[i]);
        char** paths = call_dbus_get_string_list ("com.deepin.filemanager.filedialog_x11", 
                arr[i],
                "com.deepin.filemanager.filedialog",
                "selectedFiles");
        for (int j = 0; paths[j]; ++j) {
            printf ("==>%s\n", paths[j]);
        }

        // close dialog
        call_dbus_no_return ("com.deepin.filemanager.filedialog_x11", 
                arr[i],
                "com.deepin.filemanager.filedialog",
                "deleteLater");
    }
    
    

    return 0;
}


char** call_dbus_get_string_list (const char* name, const char* addr, const char* interface, const char* methodName)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GDBusConnection) conn = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
    if (NULL != error) {
        printf ("connect error: %s\n", error->message);
        return NULL;
    }

    GVariant* result = g_dbus_connection_call_sync (conn, name, addr, interface, methodName, NULL, G_VARIANT_TYPE("(as)"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
    if (NULL != error) {
        printf ("call error: %s\n", error->message);
        return NULL;
    }

    int i = 0;
    gchar** paths;
    g_variant_get(result, "(^as)", &paths);
    for (i = 0; paths[i]; ++i);
    gchar** res = (char**) g_malloc0 (sizeof (char*) * (i + 1));
    if (!res) return NULL;

    for (i = 0; paths[i]; ++i) {
        res[i] = g_strdup (paths[i]);
    }

    g_variant_unref (result);

    return res;
}

void** call_dbus_get_object_list (const char* name, const char* addr, const char* interface, const char* methodName)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GDBusConnection) conn = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
    if (NULL != error) {
        printf ("connect error: %s\n", error->message);
        return NULL;
    }

    GVariant* result = g_dbus_connection_call_sync (conn, name, addr, interface, methodName, NULL, G_VARIANT_TYPE("(ao)"), G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
    if (NULL != error) {
        printf ("call error: %s\n", error->message);
        return NULL;
    }

    gsize count = g_variant_n_children(result);
    int pathCount = 0;
    for (int i = 0; i < count; ++i) {
        GVariant* child = g_variant_get_child_value (result, i);
        int len = 0;
        char** paths = g_variant_get_objv(child, &len);
        for (int j = 0; paths[j]; ++j, ++pathCount);
    }

    gchar** res = (char**) g_malloc0 (sizeof (char*) * (pathCount + 1));
    for (int i = 0, idx = 0; i < count; ++i) {
        GVariant* child = g_variant_get_child_value (result, i);
        int len = 0;
        char** paths = g_variant_get_objv(child, &len);
        for (int j = 0; paths[j]; ++j) {
            if (!paths[j]) continue;
            res[idx] = g_strdup (paths[j]);
            ++idx;
        }
    }

    g_variant_unref (result);

    return res;
}

void call_dbus_no_return (const char* name, const char* addr, const char* interface, const char* methodName)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GDBusConnection) conn = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
    if (NULL != error) {
        printf ("connect error: %s\n", error->message);
        return NULL;
    }

    GVariant* result = g_dbus_connection_call_sync (conn, name, addr, interface, methodName, NULL, G_VARIANT_TYPE_ANY, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
    if (NULL != error) {
        printf ("call error: %s\n", error->message);
        return NULL;
    }

    g_variant_unref (result);
}

