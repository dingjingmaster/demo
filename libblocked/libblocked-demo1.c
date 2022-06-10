/*************************************************************************
> FileName: libblocked-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 17 May 2022 03:52:06 PM CST
 ************************************************************************/
#include <glib.h>
#include <blockdev/blockdev.h>

int main (int argc, char* argv[])
{
    g_autoptr(GError) error = NULL;

    BDPluginSpec part_plugin = {BD_PLUGIN_PART, NULL};
    BDPluginSpec swap_plugin = {BD_PLUGIN_SWAP, NULL};
    BDPluginSpec loop_plugin = {BD_PLUGIN_LOOP, NULL};
    BDPluginSpec mdraid_plugin = {BD_PLUGIN_MDRAID, NULL};
    BDPluginSpec fs_plugin = {BD_PLUGIN_FS, NULL};
    BDPluginSpec crypto_plugin = {BD_PLUGIN_CRYPTO, NULL};

    BDPluginSpec *plugins[] = {&part_plugin, &swap_plugin, &loop_plugin, &mdraid_plugin, &fs_plugin, &crypto_plugin, NULL};

    gboolean ret = bd_switch_init_checks (FALSE, &error);
    if (!ret) {
        printf ("Error initializing libblockdev library: %s (%s, %d)", error->message, g_quark_to_string (error->domain), error->code);
        g_clear_error (&error);
    }

    ret = bd_try_init (plugins, NULL, NULL, &error);
    if (!ret) {
        if (error) {
            printf ("Error initializing libblockdev library: %s (%s, %d)", error->message, g_quark_to_string (error->domain), error->code);
            g_clear_error (&error);
        } else {
            /**
             * a missing plugin is okay, calling functions from it will fail, but
             * until that happens, life will just be great */
            for (BDPluginSpec** plugin_p = plugins; *plugin_p; plugin_p++) {
                if (!bd_is_plugin_available ((*plugin_p)->name)) {
                    printf ("Failed to load the '%s' libblockdev plugin", bd_get_plugin_name ((*plugin_p)->name));
                }
            }
        }
    }

    return 0;
}
