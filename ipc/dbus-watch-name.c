/*************************************************************************
> FileName: dbus-watch-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月16日 星期一 11时43分38秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

static void on_name_appeared (GDBusConnection* conn, const gchar* name, const gchar* name_ower, gpointer udata)
{
    printf ("在 '%s' 总线上的名字: '%s' 属于 '%s'\n", "G_BUS_TYPE_SESSION", name, name_ower);
}

static void on_name_vanished (GDBusConnection* conn, const gchar* name, gpointer udata)
{
    printf ("在 '%s' 总线上的名字: '%s' 已注销\n", "G_BUS_TYPE_SESSION", name);
}

int main (int argc, char* argv[])
{
    guint dbus_id = 0;
    GMainLoop* loop = NULL;
    char* watch_name = "org.dingjingmaster.demo.name";

    if (argc >= 2) {
        watch_name = argv[1];
    }

    dbus_id = g_bus_watch_name (G_BUS_TYPE_SESSION, watch_name,
            G_BUS_NAME_WATCHER_FLAGS_NONE | G_BUS_NAME_WATCHER_FLAGS_AUTO_START,
            on_name_appeared, on_name_vanished, NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
    g_bus_unwatch_name (dbus_id);

    return 0;
}
