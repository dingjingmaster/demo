/*************************************************************************
> FileName: double-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月03日 星期二 10时17分26秒
 ************************************************************************/

/* 约定:
 *  PT类型表示自定义的类型，比如GObject、demo_list_t
 *  p_t_get_type函数获取自定义类的类型，比如demo_dlist_get_type、g_object_get_type
 *  P_TYPE_T 宏获取自定义类的类型的宏，比如G_TYPE_OBJECT、DEMO_TYPE_DLIST
 *
 * 说明:
 *  要想让GObject库能够识别自定义的数据类型，必须提供一个p_t_get_type函数;
 *  该函数向GObject库所提供的类型管理系统提供要注册的PT类型相关的信息，
 *  其中包含PT类型的实例结构体初始化函数p_t_init和类结构体初始化函数p_t_class_init。
 *  由于首次调用 g_object_new 函数进行对象实例化时, 
 *  p_t_get_type 函数会被 g_object_new 函数调用，
 *  从而引发GObject库的类型管理系统去接受PT类型的申请并为其分配一个类型标识码作为p_t_type函数的返回值。
 *  当g_object_new函数从p_t_type函数那里获取PT类型识别码之后，便可以进行对象类型的内存分配及属性的初始化
 */

#include "double-list.h"

/* 用于让GObject库的数据类型系统识别我们自定义的类
 *  参数1: 类名
 *  参数2: 类成员函数的前缀(方法、行为)
 *  参数3: 此类型的父类型(G_TYPE_OBJECT实际是一个宏，代表g_object_get_type(void)函数)
 */
G_DEFINE_TYPE (demo_list_t, demo_dlist, G_TYPE_OBJECT);

// demo_dlist_get_type 由G_DEFINE_TYPE宏生成

static void demo_dlist_init (demo_list_t* self)
{
    g_print ("\tinit!\n");
    self->head = NULL;
    self->tail = NULL;
}

static void demo_dlist_class_init (demo_list_tClass* self)
{
    g_print ("\tclass init!\n");
}






