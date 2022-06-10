/*************************************************************************
> FileName: double-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月03日 星期二 10时17分26秒
 ************************************************************************/
#include "double-list.h"

/* 数据隐藏 */
#define DEMO_DLIST_GET_PRIVATE(obj) (\
        G_TYPE_INSTANCE_GET_PRIVATE ((obj), DEMO_TYPE_DLIST, demo_list_private_t))
typedef struct _demo_list_private_t demo_list_private_t;
typedef struct _demo_list_private_t
{
    demo_list_node_t* head;
    demo_list_node_t* tail;
};

G_DEFINE_TYPE (demo_list_t, demo_dlist, G_TYPE_OBJECT);

// demo_dlist_get_type 由G_DEFINE_TYPE宏生成

static void demo_dlist_init (demo_list_t* self)
{
    g_print ("\tinit!\n");
    // 使用私有属性
    demo_list_private_t* priv = DEMO_DLIST_GET_PRIVATE(self);
    priv->head = NULL;
    priv->tail = NULL;
}

// class_init 先于 init 执行
static void demo_dlist_class_init (demo_list_tClass* self)
{
    g_print ("\tclass init!\n");
    // 私有属性添加到结构体
    g_type_class_add_private (self, sizeof(demo_list_private_t));
}






