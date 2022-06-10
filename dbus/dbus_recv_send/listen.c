/*************************************************************************
> FileName: listen.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月06日 星期五 16时18分33秒
 ************************************************************************/
#include <stdio.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <glib.h>

static DBusHandlerResult dbus_filter (DBusConnection *connection, DBusMessage *message, void *user_data)
{
    if (dbus_message_is_signal(message, "org.share.linux", "Customize")) {
        printf("接受到消息\n");
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    if (dbus_message_is_signal(message, "org.share.linux", "Quit")) {
        printf("退出消息接收\n");
        GMainLoop *loop = (GMainLoop*) user_data;
        g_main_loop_quit(loop);

        return DBUS_HANDLER_RESULT_HANDLED;
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main()
{
    DBusConnection *connection;
    DBusError error;

    GMainLoop *loop;
    loop = g_main_loop_new(NULL, FALSE);

    dbus_error_init(&error);
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        printf("连接dbus守护进程错误: %s", error.message);
        dbus_error_free(&error);
        return 1;
    }

    dbus_bus_add_match (connection, "type='signal',interface='org.share.linux'", NULL);
    dbus_connection_add_filter (connection, dbus_filter, loop, NULL);

    dbus_connection_setup_with_g_main(connection, NULL);

    g_main_loop_run(loop);

    return 0;
}
