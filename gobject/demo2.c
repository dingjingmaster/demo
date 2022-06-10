/*************************************************************************
> FileName: demo2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月02日 星期一 18时30分18秒
 ************************************************************************/
#include "demo2/double-list.h"

// 类对象的私有属性模拟

int main (int argc, char* argv)
{
    // GObject 库的类型管理系统的初始化
    g_type_init ();

    int i;
    demo_list_t* l;

    /* 三个实例化 */
    for (i = 0; i < 3; ++ i) {
        l = g_object_new (DEMO_TYPE_DLIST, NULL);
        g_object_unref (l);
    }

    l = g_object_new (DEMO_TYPE_DLIST, NULL);
    if (G_IS_OBJECT(l))
        g_print ("this is a gobject object!\n");

    return 0;
}

