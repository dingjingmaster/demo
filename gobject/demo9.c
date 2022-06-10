/*************************************************************************
> FileName: demo9.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 10时35分53秒
 ************************************************************************/
#include "co-print.h"
#include <glib-object.h>

int main (void)
{
    g_type_init ();

    co_print_t* p = g_object_new (CO_TYPE_PRINT, "name", "aaa", NULL);

    g_signal_connect (p, "signal_printed", G_CALLBACK(co_print_print1), "打印信号测试1");
    g_signal_connect (p, "signal_printed", G_CALLBACK(co_print_print2), "打印信号测试2");

    co_print_print (p, "打印测试");

    g_object_unref (p);
    return 0;
}
