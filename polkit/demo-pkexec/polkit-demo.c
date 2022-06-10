/*************************************************************************
> FileName: polkit-demo.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 10 Jun 2022 05:36:10 PM CST
 ************************************************************************/
#include <glib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{
    gchar *args;
    gchar **env;
    guint n;
    int ret;
    gchar cwd[PATH_MAX];

    ret = 1;
    args = NULL;
    env = NULL;

    if (getcwd (cwd, sizeof cwd) == NULL) {
        g_printerr ("Error getting cwd: %s\n", g_strerror (errno));
        goto out;
    }

    args = g_strjoinv (" ", argv);

    g_print ("In pk-example-frobnicate\n");
    g_print ("uid:           %d\n", getuid ());
    g_print ("euid:          %d\n", geteuid ());
    g_print ("args:         `%s'\n", args);
    g_print ("cwd:           %s\n", cwd);
    g_print ("environment:\n");

    env = g_listenv ();
    for (n = 0; env[n] != NULL; n++) {
        g_print ("  %s=%s\n", env[n], g_getenv (env[n]));
    }

    ret = 0;

out:
    g_free (args);
    g_strfreev (env);

    return ret;
}


