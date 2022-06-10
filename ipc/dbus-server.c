/*************************************************************************
> FileName: dbus-server.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月16日 星期一 14时11分37秒
 ************************************************************************/
#include <gio/gio.h>
#include <stdio.h>

static const gchar nodeinfo_xml[] = 
    "<node>"
    "   <interface name='org.dingjing.gdbus.demo1'>"
    "       <annotation name='org.dingjingmaster.gdbus.Annotation' value='OnInterface'/>"
    "       <annotation name='org.dingjingmaster.gdbus.Annotation' value='AlsoOnInterface'/>"
    "       <method name='HelloWorld'>"
    "           <annotation name='org.dingjingmaster.gdbus.Annotation' value='OnMethod'/>"
    "           <arg type='s' name='greeting' direction='in'/>"
    "           <arg type='s' name='response' direction='out'/>"
    "       </method>"
    "       <method name='EmitSignal'>"
    "           <arg type='d' name='speed_in_mph' direction='in'>"
    "               <annotation name='org.dingjingmaster.gdbus.Annotation' value='OnArg'/>"
    "           </arg>"
    "       </method>"
    "       <method name='GimmeStdout'/>"
    "       <signal name='VelocityChanged'>"
    "           <annotation name='org.dingjingmaster.gdbus.Annotation' value='Onsignal'/>"
    "           <arg type='d' name='speed_in_mph'/>"
    "           <arg type='s' name='speed_as_string'>"
    "               <annotation name='org.dingjingmaster.gdbus.Annotation' value='OnArg_NonFirst'/>"
    "           </arg>"
    "       </signal>"
    "       <property type='s' name='FluxCapicitorName' access='read'>"
    "           <annotation name='org.dingjingmaster.gdbus.Annotation' value='OnProperty'>"
    "               <annotation name='org.dingjingmaster.gdbus.Annotation' value='OnAnnotation_YesThisIsCrazy'/>"
    "           </annotation>"
    "       </property>"
    "       <property type='s' name='Title' access='readwrite'/>"
    "       <property type='s' name='ReadingAlwaysThrowsError' access='read'/>"
    "       <property type='s' name='WritingAlwaysThrowsError' access='readwrite'/>"
    "       <property type='s' name='OnlyWritable' access='write'/>"
    "       <property type='s' name='Foo' access='read'/>"
    "       <property type='s' name='Bar' access='read'/>"
    "   </interface>"
    "</node>";

static gboolean handle_set_property (GDBusConnection* conn, const gchar* sender, const gchar* object_path,
        const gchar* interface_name, const gchar* property_name, GVariant* value, GError** error, gpointer udata);
static GVariant* handle_get_property (GDBusConnection* conn, const gchar* sender, const gchar* object_path,
        const gchar* interface_name, const gchar* property_name, GError** error, gpointer udata);
static void handle_method_all (GDBusConnection* conn, const gchar* sender, const gchar* object_path,
        const gchar*interface_name, const gchar* method_name, GVariant* param, GDBusMethodInvocation* inv, gpointer udata);

static gchar* global_title = NULL;
static gboolean swap_a_b = FALSE;
static GDBusNodeInfo* nodeinfo = NULL;
static const GDBusInterfaceVTable interface_vtable = {
    handle_method_all,
    handle_get_property,
    handle_set_property
};

static gboolean handle_set_property (GDBusConnection* conn, const gchar* sender, const gchar* object_path,
        const gchar* interface_name, const gchar* property_name, GVariant* value, GError** error, gpointer udata)
{
    if (g_strcmp0 (property_name, "Title") == 0) {
        if (0 != g_strcmp0 (global_title, g_variant_get_string (value, NULL))) {
            GVariantBuilder* builder;
            GError* local_error;
            g_free (global_title);
            global_title = g_variant_dup_string (value, NULL);

            local_error = NULL;
            builder = g_variant_builder_new (G_VARIANT_TYPE_ARRAY);
            g_variant_builder_add (builder, "{sv}", "Title", g_variant_new_string (global_title));
            g_dbus_connection_emit_signal (conn, NULL, object_path, "org.freedesktop.DBus.Properties", "PropertiesChanged", 
                    g_variant_new ("(sa{sv}as)", interface_name, builder, NULL), &local_error);
            g_assert_no_error (local_error);
        }
    } else if (0 == g_strcmp0 (property_name, "ReadingAlwaysThrowsError")) {
        // 读取到后做些什么
    } else if (0 == g_strcmp0 (property_name, "WritingAlwaysThrowError")) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED, "写入IO出错!", sender);
    }
    return *error == NULL;
}

static GVariant* handle_get_property (GDBusConnection* conn, const gchar* sender, const gchar* object_path,
        const gchar* interface_name, const gchar* property_name, GError** error, gpointer udata)
{
    GVariant* ret = NULL;

    if (0 == g_strcmp0 (property_name, "FluxCapiciorName")) {
        ret = g_variant_new_string ("DeLorean");
    } else if (0 == g_strcmp0 (property_name, "Title")) {
        if (NULL == global_title) {
            global_title = g_strdup ("Back to c!");
        }
        ret = g_variant_new_string (global_title);
    } else if (0 == g_strcmp0 (property_name, "ReadingAlwaysThrowError")) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED, "读取错误", sender);
    } else if (0 == g_strcmp0 (property_name, "WritingAlwaysThrowError")) {
        ret = g_variant_new_string ("There's no home like home");
    } else if (0 == g_strcmp0 (property_name, "Foo")) {
        ret = g_variant_new_string (swap_a_b?"Tock":"Tick");
    } else if (0 == g_strcmp0 (property_name, "Bar")) {
        ret = g_variant_new_string (swap_a_b?"Tick":"Tock");
    }
    return ret;
}

static void handle_method_all (GDBusConnection* conn, const gchar* sender, const gchar* object_path,
        const gchar*interface_name, const gchar* method_name, GVariant* param, GDBusMethodInvocation* inv, gpointer udata)
{
    if (0 == g_strcmp0 (method_name, "Helloworld")) {
        const gchar* greeting;
        g_variant_get (param, "(&s)", &greeting);
        if (0 == g_strcmp0 (greeting, "Return Unregistered")) {
            g_dbus_method_invocation_return_error (inv, G_IO_ERROR, G_IO_ERROR_FAILED_HANDLED, "gerror");
        } else if (0 == g_strcmp0 (greeting, "Return Registered")) {
            g_dbus_method_invocation_return_error (inv, G_DBUS_ERROR, G_DBUS_ERROR_MATCH_RULE_NOT_FOUND, "gerror");
        } else if (0 == g_strcmp0 (greeting, "Return Raw")) {
            g_dbus_method_invocation_return_dbus_error (inv, "org.dingjingmaster.gdbus.SomeErrorName", "gerror");
        } else {
            gchar* resp;
            resp = g_strdup_printf ("you greeted me with '%s'. Thanks!", greeting);
            g_dbus_method_invocation_return_value (inv, g_variant_new ("(s)", resp));
            g_free (resp);
        }
    } else if (0 == g_strcmp0 (method_name, "EmitSignal")) {
        GError* local_error;
        gdouble speed_in_mph;
        gchar* speed_as_string;

        g_variant_get (param, "(d)", &speed_in_mph);
        speed_as_string = g_strdup_printf ("%g mph!", speed_in_mph);

        local_error = NULL;
        g_dbus_connection_emit_signal (conn, NULL, object_path, interface_name, "VelocityChanged", 
                g_variant_new("(ds)", speed_in_mph, speed_as_string), &local_error);
        g_assert_no_error (local_error);
        g_free (speed_as_string);
        g_dbus_method_invocation_return_value (inv, NULL);
    } else if (0 == g_strcmp0 (method_name, "GimmeStdout")) {
        if (G_DBUS_CAPABILITY_FLAGS_UNIX_FD_PASSING & g_dbus_connection_get_capabilities(conn)) {
            GDBusMessage* reply;
            GUnixFDList* fd_list;
            GError* error;

            fd_list = g_unix_fd_list_new ();
            error = NULL;
            g_unix_fd_list_append (fd_list, STDOUT_FILENO, &error);
            g_assert_no_error (error);

            reply = g_dbus_message_new_method_reply (g_dbus_method_invocation_get_message(inv));
            g_dbus_message_set_unix_fd_list (reply, fd_list);

            error = NULL;
            g_dbus_connection_send_message (conn, reply, G_DBUS_SEND_MESSAGE_FLAGS_NONE, NULL, &error);

            g_assert_no_error (error);
            g_object_unref (inv);
            g_object_unref (fd_list);
            g_object_unref (reply);
        } else {
            g_dbus_method_invocation_return_dbus_error (inv, "org.dingjingmaster.gdbus.failed", "error");
        }
    }
}

static gboolean on_timeout_cb (gpointer udata)
{
    GDBusConnection* conn = G_DBUS_CONNECTION (udata);
    GVariantBuilder* builder = NULL;
    GVariantBuilder* ivbuilder = NULL;
    GError* error = NULL;

    swap_a_b = !swap_a_b;

    builder = g_variant_builder_new (G_VARIANT_TYPE_ARRAY);
    ivbuilder = g_variant_builder_new (G_VARIANT_TYPE("as"));
    g_variant_builder_add (builder, "{sv}", "Foo", g_variant_new_string (swap_a_b?"Tock":"Tick"));
    g_variant_builder_add (builder, "{sv}", "Bar", g_variant_new_string (swap_a_b?"Tock":"Tick"));
    g_dbus_connection_emit_signal (conn, NULL, "/org/dingjingmaster/gdbus/demo1", 
            "org.freedesktop.DBus.Properties", "PropertiesChanged",
            g_variant_new ("(sa{sv}as)", "org.dingjingmaster.gdbus.demo1, builder, ivbuilder"), &error);
    g_assert_no_error (error);

    return TRUE;
}

static void on_name_accquired (GDBusConnection* conn, const gchar* name, gpointer udata)
{
    printf ("名字'%s'注册成功!\n", name);
}

static void on_name_lost (GDBusConnection* conn, const gchar* name, gpointer udata)
{
    printf ("名字冲突!\n");
    exit(-1);
}

static void on_bus_acquired (GDBusConnection* conn, const gchar* name, gpointer udata)
{
    guint rid;

    printf ("bus acquired\n");
    rid = g_dbus_connection_register_object (conn, "/org/dingjingmaster/gdbus/demo1client",
            nodeinfo->interfaces[0], &interface_vtable, NULL/* 用户数据 */, NULL/*用户数据释放函数*/, NULL/*GError*/);
    g_assert (rid > 0);

    /* 每隔2秒保存 Foo 和 Bar 参数的值 */
    g_timeout_add_seconds (2, on_timeout_cb, conn);
}

int main (int argc, char* argv[])
{
    guint sid = 0;
    GMainLoop* loop = NULL;

    nodeinfo = g_dbus_node_info_new_for_xml (nodeinfo_xml, NULL);
    g_assert (NULL != nodeinfo);

    sid = g_bus_own_name (G_BUS_TYPE_SESSION, "org.dingjingmaster.gdbus.demo1server", G_BUS_NAME_OWNER_FLAGS_NONE,
            on_bus_acquired, on_name_accquired, on_name_lost, NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);

    g_bus_unown_name (sid);
    g_dbus_node_info_unref (nodeinfo);

    return 0;
}
