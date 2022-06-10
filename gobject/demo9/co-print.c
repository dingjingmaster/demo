/*************************************************************************
> FileName: co-print.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 10时24分47秒
 ************************************************************************/
#include "co-print.h"

G_DEFINE_TYPE (co_print_t, co_print, G_TYPE_OBJECT);

#define CO_PRINT_GET_PRIVATE(obj)(\
        G_TYPE_INSTANCE_GET_PRIVATE((obj), CO_TYPE_PRINT, co_print_private_t))

typedef struct _co_print_private_t co_print_private_t;

struct _co_print_private_t
{
    char* name;
};

enum property_list {
    PROPERTY_PRINT_0,
    PROPERTY_PRINT_NAME,
};

// 析构函数
static void co_print_dispose (GObject* obj)
{
    co_print_t* self = CO_PRINT(obj);
    co_print_private_t* priv = CO_PRINT_GET_PRIVATE (self);

    // 调用父类析构函数
    G_OBJECT_CLASS (co_print_parent_class)->dispose (obj);
}

static void co_print_finalize (GObject* obj)
{
    co_print_t* self = CO_PRINT(obj);
    co_print_private_t* priv = CO_PRINT_GET_PRIVATE (self);

    G_OBJECT_CLASS (co_print_parent_class)->finalize (obj);
}

static void co_print_set_property (GObject* obj, guint property_id, const GValue* value, GParamSpec* spec)
{
    co_print_t* self = CO_PRINT(obj);
    co_print_private_t* priv = CO_PRINT_GET_PRIVATE (self);

}

static void co_print_get_property (GObject* obj, guint property_id, GValue* value, GParamSpec* spec)
{

}

static void co_print_init (co_print_t* self)
{

}

static void co_print_class_init (co_print_tClass*  kclass)
{
    g_type_class_add_private (kclass, sizeof (co_print_private_t));

    GObjectClass* base_class = G_OBJECT_CLASS (kclass);
    base_class->set_property = co_print_set_property;
    base_class->get_property = co_print_get_property;
    base_class->dispose = co_print_dispose;
    base_class->finalize = co_print_finalize;

    GParamSpec* pspec;
    pspec = g_param_spec_string ("name", "Name", "参数名", NULL, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT);

    g_object_class_install_property (base_class, PROPERTY_PRINT_NAME, pspec);

    g_signal_new ("signal_printed", CO_TYPE_PRINT, G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS, 0, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

}

// 开始实现方法
void co_print_print (co_print_t* self, char* buffer)
{
    printf ("测试:%s\n", buffer);
    g_signal_emit_by_name (self, "signal_printed");
}

void co_print_print1 (gpointer gp, gpointer udata)
{
    printf ("print1:%s\n", (char*)udata);
}

void co_print_print2 (gpointer gp, gpointer udata)
{
    printf ("print2:%s\n", (char*)udata);
}
