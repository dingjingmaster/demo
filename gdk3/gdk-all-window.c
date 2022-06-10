/*************************************************************************
> FileName: gdk-all-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年04月03日 星期五 08时56分20秒
 ************************************************************************/
#include <gdk/gdk.h>

int main (int argc, char* argv[])
{
    gdk_init (&argc, &argv);

    GList* children = gdk_window_get_children (NULL);

    printf ("%s\n", g_list_length(children));

    return 0;
}
