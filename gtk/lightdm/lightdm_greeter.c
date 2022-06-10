/*************************************************************************
> FileName: gtk3-lightdm-greeter.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月11日 星期五 14时47分13秒
 ************************************************************************/
#include <gtk/gtk.h>
#include <lightdm.h>

static void authentication_complete_cb (LightDMGreeter *greeter);
static void show_prompt_cb (LightDMGreeter *greeter, const char *text, LightDMPromptType type);

int main ()
{
    GMainLoop *main_loop;
    LightDMGreeter *greeter;

    main_loop = g_main_loop_new (NULL, TRUE);

    greeter = lightdm_greeter_new ();
    g_object_connect (greeter, "show-prompt", G_CALLBACK (show_prompt_cb), NULL);
    g_object_connect (greeter, "authentication-complete", G_CALLBACK (authentication_complete_cb), NULL);

    // Connect to LightDM daemon
    if (!lightdm_greeter_connect_to_daemon_sync (greeter, NULL))
        return EXIT_FAILURE;

    // Start authentication
    lightdm_greeter_authenticate (greeter, "dingjing", NULL);

    g_main_loop_run (main_loop);

    return EXIT_SUCCESS;
}

static void show_prompt_cb (LightDMGreeter *greeter, const char *text, LightDMPromptType type)
{
    // Show the user the message and prompt for some response
    //gchar *secret = prompt_user (text, type);
    gchar response[1024] = {0};

    // Give the result to the user
    lightdm_greeter_respond (greeter, response, NULL);
}

static void authentication_complete_cb (LightDMGreeter *greeter)
{
    // Start the session
    if (!lightdm_greeter_get_is_authenticated (greeter) ||
        !lightdm_greeter_start_session_sync (greeter, NULL, NULL))
    {
        // Failed authentication, try again
        lightdm_greeter_authenticate (greeter, NULL, NULL);
    }
}
