/*************************************************************************
> FileName: dconf-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年01月20日 星期三 20时19分44秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>

#define SYSTEM_PROXY        "org.gnome.system.proxy"
#define SYSTEM_PROXY_HTTP   "org.gnome.system.proxy.http"
#define SYSTEM_PROXY_HTTPS  "org.gnome.system.proxy.https"

void change_func        (GSettings* gs, char* key, gpointer udata);
void change_mode        (GSettings* gs, char* key, gpointer udata);

int main (int argc, char* argv[])
{
    gchar* type = NULL;
    gtk_init (&argc, &argv);

    GSettings* gsetting         = g_settings_new (SYSTEM_PROXY);
    GSettings* gsetting_http    = g_settings_new (SYSTEM_PROXY_HTTP);
    GSettings* gsetting_https   = g_settings_new (SYSTEM_PROXY_HTTPS);
    if (NULL == gsetting) {
        printf ("错误!\n");
        return -1;
    }

    // init
    type = g_settings_get_string (gsetting, "mode");
    if (0 == g_strcmp0("none", type)) {
        printf ("none\n");
    } else if (0 == g_strcmp0("manual", type)) {
        printf ("manual\n");
    } else if (0 == g_strcmp0 ("auto", type)) {
        printf ("auto\n");
    }

    g_signal_connect (gsetting,       "changed", change_mode, NULL);
    g_signal_connect (gsetting_http,  "changed", change_func, "http");
    g_signal_connect (gsetting_https, "changed", change_func, "https");

    gtk_main();

    return 0;
}

void change_mode (GSettings* gs, char* key, gpointer udata)
{
    char*       ip = NULL;
    gint32      port = 0;

    GVariant* val = g_settings_get_value (gs, key);

    printf ("%s\n", g_variant_get_type(val));
    if (0 == g_strcmp0 (key, "mode")) {
        printf ("%s\n", g_variant_get_string(val, NULL));
    }
}

void change_func (GSettings* gs, char* key, gpointer udata)
{
    char*       ip = NULL;
    gint32      port = 0;

    GVariant* val = g_settings_get_value (gs, key);
    GVariantType* type = g_variant_get_type (val);

    printf ("%s\n", g_variant_get_type(val));

    if (0 == g_strcmp0 (key, "host") || 0 == g_strcmp0 (key, "port")) {
        if (g_variant_is_of_type (val, G_VARIANT_TYPE_INT32)) {
            GVariant* key = g_settings_get_value (gs, "host");
            printf ("host:%s --- port: %u\n", g_variant_get_string (key, NULL), g_variant_get_int32 (val));
        } else if (g_variant_is_of_type (val, G_VARIANT_TYPE_STRING)) {
            GVariant* key = g_settings_get_value (gs, "port");
            printf ("host:%s --- port: %u\n", g_variant_get_string (val, NULL), g_variant_get_int32 (key));
        }
    }

    if (0 == g_strcmp0 ("http", udata)) {
        printf ("http\n");
    } else if (0 == g_strcmp0 ("https", udata)) {
        printf ("https\n");
    }
}

