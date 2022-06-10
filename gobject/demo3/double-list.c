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
};

struct _demo_list_node_t
{
    demo_list_node_t* prev;
    demo_list_node_t* next;
    void* data;
};

enum _demo_list_property_t
{
    DEMO_LIST_HEAD,
    DEMO_LIST_TAIL,
    DEMO_LIST_NODE_PREV,
    DEMO_LIST_NODE_NEXT
};

// g_object_class_install_property 函数的第二个参数值不能是0
enum _demo_property_list_t
{
    DEMO_PROPERTY_LIST_0,
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
static void demo_list_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


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
    pspec = g_param_spec_pointer("head", "HEAD node",
            "The head node of the list", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT);
    g_object_class_install_property(base_class, DEMO_PROPERTY_LIST_HEAD, pspec);

    pspec = g_param_spec_pointer ("head-prev",
            "The previous node of the head node",
            "The previous node of the head node of the double list",
            G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (base_class, DEMO_PROPERTY_LIST_HEAD_PREV, pspec);

    pspec = g_param_spec_pointer ("head-next",
            "The next node of the head node",
            "The next node of the head node of the double list",
            G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (base_class, DEMO_PROPERTY_LIST_HEAD_NEXT, pspec);

    pspec = g_param_spec_pointer ("tail",
            "Tail node",
            "The tail node of the double list",
            G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT);
    g_object_class_install_property (base_class, DEMO_PROPERTY_LIST_TAIL, pspec);

    pspec = g_param_spec_pointer ("tail-prev",
            "The previous node of the tail node",
            "The previous node of the tail node of the double list",
            G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (base_class, DEMO_PROPERTY_LIST_TAIL_PREV, pspec);

    pspec = g_param_spec_pointer ("tail-next",
            "The next node of the tail node",
            "The next node of the tail node of the double list",
            G_PARAM_READABLE | G_PARAM_WRITABLE);
    g_object_class_install_property (base_class, DEMO_PROPERTY_LIST_TAIL_NEXT, pspec);
}

demo_list_node_t* demo_list_get (demo_list_t* self, demo_list_property_t property)
{
    demo_list_node_t* node = NULL;
    demo_list_private_t* priv = DEMO_DLIST_GET_PRIVATE(self);

    switch (property) {
    case DEMO_LIST_HEAD:
        node = priv->head;
        break;
    case DEMO_LIST_TAIL:
        node = priv->tail;
        break;
    default:
        g_print ("error");
    }
    return node;
}

void demo_list_set (demo_list_t* self, demo_list_property_t property, demo_list_property_t subproperty, demo_list_node_t* node)
{
    demo_list_private_t* priv = DEMO_DLIST_GET_PRIVATE(self);

    switch (property) {
    case DEMO_LIST_HEAD:
        if (subproperty == DEMO_LIST_NODE_PREV)
            priv->head->prev = node;
        else if (subproperty == DEMO_LIST_NODE_NEXT)
            priv->head->next = node;
        break;
    case DEMO_LIST_TAIL:
        if (subproperty == DEMO_LIST_NODE_PREV)
            priv->tail->prev = node;
        else if (subproperty = DEMO_LIST_NODE_NEXT)
            priv->tail->next = node;
        break;
    default:
        g_print ("error");
    }
}

static void demo_list_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    demo_list_t *self = DEMO_LIST(object);
    demo_list_private_t *priv = DEMO_DLIST_GET_PRIVATE (self);

    switch (property_id) {
    case DEMO_PROPERTY_LIST_HEAD:
        priv->head = g_value_get_pointer (value);
        break;
    case DEMO_PROPERTY_LIST_HEAD_PREV:
        priv->head->prev = g_value_get_pointer (value);
        break;
    case DEMO_PROPERTY_LIST_HEAD_NEXT:
        priv->head->next = g_value_get_pointer (value);
        break;
    case DEMO_PROPERTY_LIST_TAIL:
        priv->tail = g_value_get_pointer (value);
        break;
    case DEMO_PROPERTY_LIST_TAIL_PREV:
        priv->tail->prev = g_value_get_pointer (value);
        break;
    case DEMO_PROPERTY_LIST_TAIL_NEXT:
        priv->tail->next = g_value_get_pointer (value);
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
    case DEMO_PROPERTY_LIST_HEAD:
        g_value_set_pointer (value, priv->head);
        break;
    case DEMO_PROPERTY_LIST_HEAD_PREV:
        g_value_set_pointer (value, priv->head->prev);
        break;
    case DEMO_PROPERTY_LIST_HEAD_NEXT:
        g_value_set_pointer (value, priv->head->next);
        break;
    case DEMO_PROPERTY_LIST_TAIL:
        g_value_set_pointer (value, priv->tail);
        break;
    case DEMO_PROPERTY_LIST_TAIL_PREV:
        g_value_set_pointer (value, priv->tail->prev);
        break;
    case DEMO_PROPERTY_LIST_TAIL_NEXT:
        g_value_set_pointer (value, priv->tail->next);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

