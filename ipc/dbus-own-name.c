/*************************************************************************
> FileName: dbus-own-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月16日 星期一 11时04分59秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

static void dbus_acquired (GDBusConnection* conn, const gchar* name, gpointer udata)
{
    printf ("总线上导出一些对象的地方！\n");
}

static void dbus_name_acquired (GDBusConnection* conn, const gchar* name, gpointer udata)
{
    printf ("名字可用！\n");
}

int main (int argc, char* argv[])
{
    guint dbus_id = 0;
    GMainLoop* loop = NULL;

#if 1
    // 登录会话消息总线
    // 在指定总线上获取名字，并在获取名字时分别调用 name_acquired_handler(名字可以获取) name_lost_handler(名称无法获取)
    dbus_id = g_bus_own_name (G_BUS_TYPE_SESSION, "org.dingjingmaster.demo.name",
            G_BUS_NAME_OWNER_FLAGS_NONE | G_BUS_NAME_OWNER_FLAGS_REPLACE | G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT,
            dbus_acquired, dbus_name_acquired, NULL, NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);
#endif

    g_main_loop_run (loop);
    // 不在拥有分配的 name 
    g_bus_unown_name (dbus_id);

    return 0;
}
