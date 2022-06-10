/*************************************************************************
> FileName: double-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月03日 星期二 10时17分26秒
 ************************************************************************/
#include "double-list.h"

typedef struct _demo_list_private_t demo_list_private_t;
typedef enum _demo_list_property_t demo_list_property_t;
typedef struct _demo_list_private_t
{
    demo_list_node_t* head;
    demo_list_node_t* tail;
    char* data;
};

struct _demo_list_node_t
{
    demo_list_node_t* prev;
    demo_list_node_t* next;
};

// g_object_class_install_property 函数的第二个参数值不能是0
enum _demo_property_list_t
{
    DEMO_PROPERTY_LIST_0,
    DEMO_PROPERTY_LIST_DATA,    /* demo4测试数据 */
    DEMO_PROPERTY_LIST_HEAD,
    DEMO_PROPERTY_LIST_HEAD_PREV,
    DEMO_PROPERTY_LIST_HEAD_NEXT,
    DEMO_PROPERTY_LIST_TAIL,
    DEMO_PROPERTY_LIST_TAIL_PREV,
    DEMO_PROPERTY_LIST_TAIL_NEXT
};
// demo_dlist_get_type 由G_DEFINE_TYPE宏生成
G_DEFINE_TYPE (demo_list_t, demo_dlist, G_TYPE_OBJECT);

/* 基类指针转换为 demo_list_t 指针 */
#define DEMO_LIST(obj)\
    G_TYPE_CHECK_INSTANCE_CAST ((obj), DEMO_TYPE_DLIST, demo_list_t)

/* 数据隐藏 */
#define DEMO_DLIST_GET_PRIVATE(obj) (\
    G_TYPE_INSTANCE_GET_PRIVATE ((obj), DEMO_TYPE_DLIST, demo_list_private_t))

demo_list_node_t* demo_list_get (demo_list_t* self, demo_list_property_t property);
void demo_list_set (demo_list_t* self, demo_list_property_t property, demo_list_property_t subproperty, demo_list_node_t* node);
static void demo_list_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void demo_list_set_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);


static void demo_dlist_init (demo_list_t* self)
{
}

// class_init 先于 init 执行
static void demo_dlist_class_init (demo_list_tClass* self)
{
    g_print ("\tclass init!\n");

    // 私有属性添加到结构体
    g_type_class_add_private (self, sizeof(demo_list_private_t));

    // 覆盖GObject类的指针
    // 类是由实例结构体+类结构体构成，对象的属性在实例结构体中，所有对象共享数据在类结构体中
    // set/get_property 可以被GObject类及其子类所有对象共享
    GObjectClass* base_class = G_OBJECT_CLASS(self);
    base_class->set_property = demo_list_set_property;
    base_class->get_property = demo_list_get_property;

    // 安装属性
    GParamSpec* pspec;
    pspec = g_param_spec_pointer("data", "demo4",
            "test data", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT);
    g_object_class_install_property(base_class, DEMO_PROPERTY_LIST_DATA, pspec);
}

static void demo_list_set_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    demo_list_t *self = DEMO_LIST(object);
    demo_list_private_t *priv = DEMO_DLIST_GET_PRIVATE (self);

    switch (property_id) {
    case DEMO_PROPERTY_LIST_DATA:
        priv->data = g_value_get_pointer(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}
 
static void demo_list_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    demo_list_t* self = DEMO_LIST (object);
    demo_list_private_t *priv = DEMO_DLIST_GET_PRIVATE (self);
         
    switch (property_id) {
    case DEMO_PROPERTY_LIST_DATA:
        g_value_set_pointer (value, priv->data);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

void demo_list_printf (demo_list_t* self)
{
    char* data;
    g_object_get(G_OBJECT(self), "data", &data, NULL);
    g_print ("%s\n", data);
}

