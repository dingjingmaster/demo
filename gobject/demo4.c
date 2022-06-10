/*************************************************************************
> FileName: demo3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月02日 星期一 18时30分18秒
 ************************************************************************/
#include "demo4/double-list.h"

// 类对象的私有属性外部访问

int main (int argc, char* argv)
{
    // GObject 库的类型管理系统的初始化
    g_type_init ();

    int i;
    demo_list_t* l;

    /* 这是一个多参数函数，初始化数据的成员，这样初始化需要 p_t_set_property 和 p_t_get_property 函数 */
    l = g_object_new (DEMO_TYPE_DLIST, "data", "demo4 test string", NULL);
    if (G_IS_OBJECT(l))
        g_print ("this is a gobject object!\n");

    demo_list_printf (l);

    return 0;
}

