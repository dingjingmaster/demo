/*************************************************************************
> FileName: demo3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月02日 星期一 18时30分18秒
 ************************************************************************/
#include "demo3/double-list.h"

// 类对象的私有属性外部访问

int main (int argc, char* argv)
{
    // GObject 库的类型管理系统的初始化
    g_type_init ();

    int i;
    GValue val = {0};
    demo_list_t* l;

    /* 三个实例化 */
    for (i = 0; i < 3; ++ i) {
        l = g_object_new (DEMO_TYPE_DLIST,
                "head", NULL,
                "tail", NULL, NULL);
        g_object_unref (l);
    }

    /* 这是一个多参数函数，初始化数据的成员，这样初始化需要 p_t_set_property 和 p_t_get_property 函数 */
    l = g_object_new (DEMO_TYPE_DLIST, "head", NULL, "tail", NULL, NULL);
    if (G_IS_OBJECT(l))
        g_print ("this is a gobject object!\n");

    // 初始化
    g_value_init(&val, G_TYPE_POINTER);

    // 这样获取值
    g_object_get_property(G_OBJECT(l), "head", &val);

    // 这样设置值
    g_object_set_property(G_OBJECT(l), "head", &val);

    // 另外 g_object_set 和 g_object_get 可以一次性取多个值，与 g_object_new 类似

    g_value_unset (&val);

    return 0;
}

