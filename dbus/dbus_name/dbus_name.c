/*************************************************************************
> FileName: bus_name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月06日 星期五 16时02分26秒
 ************************************************************************/
#include <stdio.h>
#include <dbus/dbus.h>

int main()
{
    DBusConnection *connection;
    DBusError error;

    char *name = "org.share.linux";

    dbus_error_init(&error);
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        printf("dbus 连接错误: %s", error.message);
        dbus_error_free(&error);
        return 1;
    }

    dbus_bool_t ret = dbus_bus_name_has_owner(connection,name,&error);
    if (dbus_error_is_set(&error)) {
        dbus_error_free(&error);
        printf("DBus Error: %s\n", error.message);
        return 1;
    }

    if (ret == FALSE) {
        printf("Bus name %s 未被使用，可以选用\n",name);
        int request_name_reply = dbus_bus_request_name(connection,name, DBUS_NAME_FLAG_DO_NOT_QUEUE, &error);
        if (dbus_error_is_set(&error)) {
            dbus_error_free(&error);
            printf("Error requesting a bus name: %s\n",error.message);
            return 1;
        }

        if (request_name_reply == DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
            printf("Bus name %s 使用成功!\n",name);
            return 0;
        } else {
            printf("Bus name %s 使用失败\n",name);
            return 1;
        }
    } else {
        printf("bus name %s 被使用\n", name);
        return 1;
    }

    return 0;
}
