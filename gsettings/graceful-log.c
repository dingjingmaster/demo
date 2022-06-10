/*************************************************************************
> FileName: graceful-log.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 03 May 2022 12:12:28 PM CST
 ************************************************************************/

#include <glib.h>
#include <gio/gio.h>

static void change (GSettings* s, char* key, gpointer udata)
{
    if (NULL == key || NULL == s) return;

    printf ("key: %s, value: %s\n", key, g_settings_get_boolean(s, key) ? "true" : "false");
}

int main (int argc, char* argv[])
{
    GMainLoop* loop = g_main_loop_new (NULL, TRUE);
    GSettingsSchemaSource* gss = g_settings_schema_source_get_default ();
    if (gss) {
        GSettingsSchema* schema = g_settings_schema_source_lookup (gss, "org.graceful.settings.log", TRUE);
        if (schema) {
            GSettings* gs = g_settings_new_full (schema, NULL, NULL);
            if (gs) {
                g_signal_connect (gs, "changed", change, NULL);
            }
        }
    }

    if (loop) {
        g_main_loop_run (loop);
    }

    return 0;
}
