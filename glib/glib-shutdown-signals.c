/*************************************************************************
> FileName: glib-shutdown-signals.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年05月20日 星期四 08时26分43秒
 ************************************************************************/
#include <gio/gio.h>
#include <gtk/gtk.h>

static void system_sleep (GDBusConnection *connection, const gchar *senderName, const gchar *objectPath, const gchar *interfaceName, const gchar *signalName, GVariant *parameters, gpointer udata)
{
    printf ("开始休眠!!!\n\n");
//    static_cast<FileOperationManager*>(udata)->pauseAllOperation();
}

int main (int argc, char* argv[])
{
    gtk_init (&argc, &argv);

    GDBusConnection* pconnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    if (pconnection) {
        printf ("dbus ok\n\n");
        g_dbus_connection_signal_subscribe(pconnection,
                           "org.freedesktop.login1",
                           "org.freedesktop.login1.Manager",
                           "PrepareForSleep",
                           "/org/freedesktop/login1", NULL,
                           G_DBUS_SIGNAL_FLAGS_NONE,
                           system_sleep, NULL, NULL);
    } else {
        printf ("dbus error!!!\n\n");
    }

    gtk_main();

    return 0;
}
