/*************************************************************************
> FileName: gsettings-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月30日 星期一 23时21分00秒
 ************************************************************************/
#include <gio/gio.h>

int main (int argc, char* argv)
{
    char** keys = NULL;
    GSettings* gs = NULL;

    //gs = g_settings_new ("org.freedesktop.ibus");
    gs = g_settings_new_with_path ("org.freedesktop.ibus.general", "/desktop/ibus/general/");

    keys = g_settings_list_keys (gs);
    if (NULL == keys) printf ("null");
    for (int i = 0; keys[i] != NULL; ++i) {
        printf ("%s --- \n", keys[i]);
    }

    g_strfreev (keys);

    g_object_unref (gs);

    return 0;
}
