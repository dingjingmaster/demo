/*************************************************************************
> FileName: send.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月06日 星期五 16时25分16秒
 ************************************************************************/
#include <stdio.h>
#include <dbus/dbus.h>

static void send_config(DBusConnection *connection)
{
    DBusMessage *message;
    message = dbus_message_new_signal ("/org/share/linux", "org.share.linux", "Customize");

    dbus_connection_send (connection, message, NULL);
    dbus_message_unref (message);
}

static void send_quit (DBusConnection *connection)
{
    DBusMessage *message;

    message = dbus_message_new_signal ("/org/share/linux", "org.share.linux", "Quit");

    dbus_connection_send (connection, message, NULL);
    dbus_message_unref (message);
}

int main (int argc, char **argv)
{
    int i = 0;

    DBusConnection *connection;
    DBusError error;

    dbus_error_init (&error);
    connection = dbus_bus_get (DBUS_BUS_SESSION, &error);
    if (!connection) {
        printf ("Failed to connect to the D-BUS daemon: %s", error.message);
        dbus_error_free (&error);
        return 1;
    }

    if (argc == 1) {
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-c")) {
            send_config(connection);
        } else if (!strcmp(argv[i], "-q")) {
            send_quit(connection);
        }
    }

    return 0;
}
