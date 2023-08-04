/*************************************************************************
> FileName: gdbus-list-path.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 03 Aug 2023 07:00:40 PM CST
 ************************************************************************/
#include <gio/gio.h>

int main (int argc, char* argv[])
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GDBusConnection) conn = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
    if (NULL != error) {
        printf ("connect error: %s\n", error->message);
        return -1;
    }

    GVariant* result = g_dbus_connection_call_sync (conn,
                            "org.freedesktop.DBus",                        // 目标对象名称
                            "/",                            // 目标对象路径
                            "org.freedesktop.DBus",                       
                            "ListNames",                    // 方法名称
                            NULL,                           // 参数
                            G_VARIANT_TYPE("(as)"),         // 返回类型
                            G_DBUS_CALL_FLAGS_NONE,
                            -1,
                            NULL,
                            &error);
    if (NULL != error) {
        printf ("call error: %s\n", error->message);
        return -1;
    }

    gchar** paths;
    g_variant_get(result, "(^as)", &paths);
    for (int i = 0; paths[i]; ++i) {
        printf ("%s\n", paths[i]);
    }
    g_variant_unref (result);


    return 0;
}
