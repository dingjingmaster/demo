#ifndef MY_OBJECT_H
#define MY_OBJECT_H

#include <glib-object.h>

G_BEGIN_DECLS

#define MY_TYPE_OBJECT (my_object_get_type())

typedef struct _MyObject MyObject;
typedef struct _MyObjectClass MyObjectClass;

// 类型声明
GType my_object_get_type(void);
MyObject *my_object_cast(GObject *obj);

// 公共 API
MyObject *my_object_new(void);
const gchar *my_object_get_name(MyObject *self);
void my_object_set_name(MyObject *self, const gchar *name);

G_END_DECLS

#endif /* MY_OBJECT_H */ 