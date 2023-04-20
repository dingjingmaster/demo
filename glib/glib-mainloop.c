/*************************************************************************
> FileName: glib-mainloop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 17 Apr 2023 09:56:19 AM CST
 ************************************************************************/
#include <glib.h>

int main (int argc, char* argv[])
{
    printf("start...\n");

    GMainLoop* loop = g_main_loop_new(NULL, TRUE);
    //GMainLoop* loop = g_main_loop_new(NULL, FALSE);

    printf ("running...\n");

    g_main_loop_run(loop);

    printf("end!\n");
}
