/*************************************************************************
> FileName: my-iusb.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月03日 星期二 17时26分01秒
 ************************************************************************/
#include "my-iusb.h"

/* 接口定义 */

G_DEFINE_INTERFACE (my_iusb_t, my_iusb, G_TYPE_INVALID);

/* 是 G_DEFAULT_INTERFACE 宏展开代码中声明的一个函数，可以放置接口的一些初始化功能 */
static void my_iusb_default_init (my_iusb_interface_t* iface)
{

}

gchar* my_iusb_read (my_iusb_t* self)
{
    g_return_if_fail (MY_IS_IUSB(self));
    MY_IUSB_GET_INTERFACE (self)->read (self);
}

void my_iusb_write (my_iusb_t* self, const gchar* str)
{
    g_return_if_fail (MY_IS_IUSB(self));
    MY_IUSB_GET_INTERFACE (self)->write (self, str);
}
