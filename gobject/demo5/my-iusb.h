/*************************************************************************
> FileName: my-iusb.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月03日 星期二 17时18分39秒
 ************************************************************************/
#ifndef _MY-IUSB_H
#define _MY-IUSB_H
#include <glib-object.h>

/* 接口声明 */

typedef struct _my_iusb_t my_iusb_t;
typedef struct _my_iusb_interface_t my_iusb_interface_t;

#define MY_TYPE_IUSB (my_iusb_get_type())
#define MY_IUSB(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MY_TYPE_IUSB, my_iusb_t))
#define MY_IS_IUSB(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MY_TYPE_IUSB))
#define MY_IUSB_GET_INTERFACE(obj) (\
        G_TYPE_INSTANCE_GET_INTERFACE((obj), MY_TYPE_IUSB, my_iusb_interface_t))

struct _my_iusb_interface_t
{
    GTypeInterface parent_interface;
    gchar* (*read) (my_iusb_t* self);
    void (*write) (my_iusb_t* self, const gchar* str);
};

GType my_iusb_get_type (void);

gchar* my_iusb_read (my_iusb_t* self);
void my_iusb_write (,y_iusb_t* self, const gchar* str);

#endif
