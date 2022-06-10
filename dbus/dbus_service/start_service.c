/*************************************************************************
> FileName: start_service.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月09日 星期一 10时17分02秒
 ************************************************************************/
#include <stdio.h>
#include <dbus/dbus.h>

int main()
{
    DBusConnection *connection;
    DBusError error;
    DBusMessage *message;

    const char *service_name = "org.share.demo1";
    dbus_uint32_t flag; /* Currently this is not used by DBus, they say it is for futur expansion*/
    dbus_bool_t result;

    dbus_error_init(&error);
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        printf("Error getting dbus connection: %s\n",error.message);
        dbus_error_free(&error);
        dbus_connection_unref(connection);
        return 0;
    }

    message = dbus_message_new_method_call("org.freedesktop.DBus",
            "/org/freedesktop/DBus",
            "org.freedesktop.DBus",
            "StartServiceByName");

    if (!message) {
        printf("Error creating DBus message\n");
        dbus_connection_unref(connection);
        return 0;
    }

    dbus_message_set_no_reply(message, TRUE);

    /* Append the argument to the message, must ends with DBUS_TYPE_UINT32 */
    dbus_message_append_args(message,
            DBUS_TYPE_STRING,
            &service_name,
            DBUS_TYPE_UINT32,
            &flag,
            DBUS_TYPE_INVALID);

    result = dbus_connection_send(connection, message, NULL);
    if (result == TRUE) {
        printf("Successfully activating the %s service\n",service_name);
    } else {
        printf("Failed to activate the %s service\n",service_name);
    }

    dbus_message_unref(message);
    dbus_connection_unref(connection);

    return 0;
}
