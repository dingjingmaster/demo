/*************************************************************************
> FileName: main_event_loop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月30日 星期一 21时17分49秒
 ************************************************************************/
#include <glib.h>

int main (int argc, char* argv[])
{
    GMainLoop* ml = NULL;

    ml = g_main_loop_new (NULL, FALSE);

    g_main_loop_run (ml);

    return 0;
}
