#include <glib-object.h>
#include "demo13.h"

// 定义对象结构体
typedef struct _MyObject {
    GObject parent_instance;  // 包含 ref_count 和 g_type_instance
    gchar *name;             // 实例数据
    gint instance_data;      // 实例数据
    GList *instance_list;    // 实例数据
} MyObject;

typedef struct _MyObjectClass {
    GObjectClass parent_class;  // 包含虚函数表和类型信息
    gint class_data;           // 类数据
    gchar *class_name;         // 类数据
    GList *class_list;         // 类数据
} MyObjectClass;

// 前向声明
static void my_object_class_init(MyObjectClass *class);
static void my_object_init(MyObject *self);

// 实例销毁函数
static void
my_object_dispose(GObject *object)
{
    MyObject *self = MY_OBJECT(object);

    // 1. 断开所有信号连接
    g_signal_handlers_disconnect_matched(object,
                                       G_SIGNAL_MATCH_DATA,
                                       0, 0, NULL, NULL, NULL);

    // 2. 清理实例数据
    if (self->instance_list) {
        g_list_free(self->instance_list);
        self->instance_list = NULL;
    }

    // 3. 调用父类的 dispose 函数
    G_OBJECT_CLASS(my_object_parent_class)->dispose(object);
}

// 实例最终清理函数
static void
my_object_finalize(GObject *object)
{
    MyObject *self = MY_OBJECT(object);

    // 1. 释放实例数据
    g_free(self->name);
    self->name = NULL;

    // 2. 调用父类的 finalize 函数
    G_OBJECT_CLASS(my_object_parent_class)->finalize(object);
}

// 基类初始化函数
static void
my_object_base_init(MyObjectClass *class)
{
    static gboolean initialized = FALSE;

    if (!initialized) {
        class->class_data = 0;
        class->class_name = g_strdup("MyObjectClass");
        class->class_list = NULL;
        g_print("基类初始化完成\n");
        initialized = TRUE;
    }
}

// 基类清理函数
static void
my_object_base_finalize(MyObjectClass *class)
{
    g_print("清理基类数据\n");
    class->class_data = 0;
    g_free(class->class_name);
    class->class_name = NULL;
    if (class->class_list) {
        g_list_free(class->class_list);
        class->class_list = NULL;
    }
}

// 类清理函数
static void
my_object_class_finalize(MyObjectClass *class)
{
    g_print("类清理完成\n");
}

// 类型检查函数
static gboolean my_object_is_a(GType type)
{
    return g_type_is_a(type, MY_TYPE_OBJECT);
}

// 类型转换函数
MyObject *my_object_cast(GObject *obj)
{
    g_return_val_if_fail(my_object_is_a(G_OBJECT_TYPE(obj)), NULL);
    return (MyObject *)obj;
}

// 类型检查宏
#define MY_OBJECT(obj) (my_object_cast(G_OBJECT(obj)))
#define MY_IS_OBJECT(obj) (my_object_is_a(G_OBJECT_TYPE(obj)))

// 类型注册函数
static GType my_object_type = 0;

GType my_object_get_type(void)
{
    if (G_UNLIKELY(my_object_type == 0)) {
        static const GTypeInfo type_info = {
            sizeof(MyObjectClass),                // class_size
            (GBaseInitFunc)my_object_base_init,   // base_init
            (GBaseFinalizeFunc)my_object_base_finalize,  // base_finalize
            (GClassInitFunc)my_object_class_init, // class_init
            (GClassFinalizeFunc)my_object_class_finalize, // class_finalize
            NULL,                                 // class_data
            sizeof(MyObject),                     // instance_size
            0,                                    // n_preallocs
            (GInstanceInitFunc)my_object_init,    // instance_init
            NULL                                  // value_table
        };

        my_object_type = g_type_register_dynamic(G_TYPE_OBJECT,
                                               "MyObject",
                                               &type_info,
                                               0);
    }
    return my_object_type;
}

// 属性枚举
enum {
    PROP_0,
    PROP_NAME,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };

// 获取属性
static void
my_object_get_property(GObject *object,
                      guint property_id,
                      GValue *value,
                      GParamSpec *pspec)
{
    MyObject *self = MY_OBJECT(object);

    switch (property_id) {
        case PROP_NAME:
            g_value_set_string(value, self->name);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

// 设置属性
static void
my_object_set_property(GObject *object,
                      guint property_id,
                      const GValue *value,
                      GParamSpec *pspec)
{
    MyObject *self = MY_OBJECT(object);

    switch (property_id) {
        case PROP_NAME:
            g_free(self->name);
            self->name = g_value_dup_string(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
}

// 初始化实例
static void
my_object_init(MyObject *self)
{
    self->name = g_strdup("默认名称");
    self->instance_data = 0;
    self->instance_list = NULL;
}

// 初始化类
static void
my_object_class_init(MyObjectClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS(class);

    // 设置虚函数
    object_class->dispose = my_object_dispose;
    object_class->finalize = my_object_finalize;
    object_class->get_property = my_object_get_property;
    object_class->set_property = my_object_set_property;

    // 初始化类数据
    class->class_data = 42;
    class->class_name = g_strdup("MyObjectClass");
    class->class_list = NULL;

    properties[PROP_NAME] = g_param_spec_string("name",
                                              "名称",
                                              "对象的名称",
                                              NULL,
                                              G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, properties);
}

// 创建新实例的函数
MyObject *
my_object_new(void)
{
    return g_object_new(MY_TYPE_OBJECT, NULL);
}

// 获取名称的函数
const gchar *
my_object_get_name(MyObject *self)
{
    g_return_val_if_fail(MY_IS_OBJECT(self), NULL);
    return self->name;
}

// 设置名称的函数
void
my_object_set_name(MyObject *self, const gchar *name)
{
    g_return_if_fail(MY_IS_OBJECT(self));
    g_object_set(self, "name", name, NULL);
} 
