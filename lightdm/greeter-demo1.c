/*************************************************************************
> FileName: greeter-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 12 Apr 2022 09:31:16 AM CST
 ************************************************************************/
#include <lightdm.h>
#include <glib.h>

static void authentication_complete_cb (LightDMGreeter *greeter);
static void show_prompt_cb (LightDMGreeter *greeter, const char *text, LightDMPromptType type);

int main ()
{
    GMainLoop *main_loop;
    LightDMGreeter *greeter;
    g_autoptr(GError) error = NULL;

    main_loop = g_main_loop_new (NULL, TRUE);

    greeter = lightdm_greeter_new ();
    g_object_connect (greeter, "show-prompt", G_CALLBACK (show_prompt_cb), NULL);
    g_object_connect (greeter, "authentication-complete", G_CALLBACK (authentication_complete_cb), NULL);

    // Connect to LightDM daemon
    if (!lightdm_greeter_connect_to_daemon_sync (greeter, &error)) {
        printf ("error: %s\n", error->message);
        return EXIT_FAILURE;
    }

    // Start authentication
    lightdm_greeter_authenticate (greeter, "dingjing", &error);
    if (error) {
        printf ("error: %s\n", error->message);
    }

    g_main_loop_run (main_loop);

    return EXIT_SUCCESS;
}

static void show_prompt_cb (LightDMGreeter *greeter, const char *text, LightDMPromptType type)
{
    // Show the user the message and prompt for some response
    //gchar *secret = prompt_user (text, type);
    printf ("text: %s -- type: %d\n", text, type);

    // Give the result to the user
    //lightdm_greeter_respond (greeter, response, NULL);
}

static void authentication_complete_cb (LightDMGreeter *greeter)
{
    // Start the session
    if (!lightdm_greeter_get_is_authenticated (greeter) || !lightdm_greeter_start_session_sync (greeter, "ukui", NULL)) {
        // Failed authentication, try again
        lightdm_greeter_authenticate (greeter, "dingjing", NULL);
    }
}
