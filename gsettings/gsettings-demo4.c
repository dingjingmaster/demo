/*************************************************************************
> FileName: gsettings-demo4.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月30日 星期一 23时21分00秒
 ************************************************************************/
#include <gio/gio.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    GSettings* gs = g_settings_new("org.gnome.shell.keybindings");
    if (gs) {
        GVariant* variant = g_variant_new_strv((const char*[]) {NULL}, -1);
        if (variant) {
            gboolean ret = g_settings_set_value(gs, "show-screenshot-ui", variant);
            printf ("set %s!\n", ret ? "true" : "false");
            g_settings_sync();
            g_variant_unref(variant);
        }
        g_object_unref (gs);
    }

    return 0;
}
