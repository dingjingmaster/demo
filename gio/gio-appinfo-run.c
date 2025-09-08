/*************************************************************************
> FileName: gio-appinfo-run.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月08日 星期一 17时03分40秒
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>
#include <gdk/gdk.h>
#include <gio/gdesktopappinfo.h>


int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("%s <file>\n", argv[0]);
        return 0;
    }

    GFile* file = g_file_new_for_uri(argv[1]);

    char* scheme = g_file_get_uri_scheme (file);
    printf ("schema: %s\n", scheme);
    GAppInfo* application = g_app_info_get_default_for_uri_scheme (scheme);
    if (application) {
        GError* error = NULL;
        GList* uris = NULL;
        uris = g_list_prepend(uris, argv[1]);
        GdkDisplay *display = gdk_display_get_default ();
        GdkAppLaunchContext *launchContext = gdk_display_get_app_launch_context (display);;
        gboolean result = g_desktop_app_info_launch_uris_as_manager (G_DESKTOP_APP_INFO (application),
                                uris,
                                G_APP_LAUNCH_CONTEXT (launchContext),
                                G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD
                                | G_SPAWN_STDERR_TO_DEV_NULL | G_SPAWN_STDOUT_TO_DEV_NULL,
                                NULL, NULL,
                                NULL, application,
                                &error);
        printf ("launch res: %s\n", (result ? "true" : "false"));
        if (error) {
            printf ("error: %s\n", error->message);
        }
    }

    return 0;
}
