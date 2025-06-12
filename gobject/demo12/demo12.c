#include <glib-object.h>
#include "demo12.h"

// 属性枚举
enum {
    PROP_0,
    PROP_NAME,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL, };



// 定义对象结构体
typedef struct _MyObject {
    GObject parent_instance;
    gchar *name;
} MyObject;

typedef struct _MyObjectClass {
    GObjectClass parent_class;
} MyObjectClass;


static void
my_object_set_property(GObject *object,
                      guint property_id,
                      const GValue *value,
                      GParamSpec *pspec);

static void
my_object_get_property(GObject *object,
                      guint property_id,
                      GValue *value,
                      GParamSpec *pspec);

// 初始化实例
static void
my_object_init(MyObject *self)
{
    self->name = g_strdup("默认名称");
}

// 初始化类
static void
my_object_class_init(MyObjectClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS(class);

    object_class->get_property = my_object_get_property;
    object_class->set_property = my_object_set_property;

    properties[PROP_NAME] = g_param_spec_string("name",
                                              "名称",
                                              "对象的名称",
                                              NULL,
                                              G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, N_PROPERTIES, properties);
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
            sizeof(MyObjectClass),
            NULL,           // base_init
            NULL,           // base_finalize
            (GClassInitFunc)my_object_class_init,
            NULL,           // class_finalize
            NULL,           // class_data
            sizeof(MyObject),
            0,              // n_preallocs
            (GInstanceInitFunc)my_object_init,
            NULL            // value_table
        };

        my_object_type = g_type_register_static(G_TYPE_OBJECT,
                                              "MyObject",
                                              &type_info,
                                              0);
    }
    return my_object_type;
}

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
