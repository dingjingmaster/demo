/*************************************************************************
> FileName: co-print.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月20日 星期五 10时15分27秒
 ************************************************************************/
#ifndef _CO-PRINT_H
#define _CO-PRINT_H
#include <glib-object.h>

#define CO_TYPE_PRINT (co_print_get_type())
#define CO_PRINT(obj) G_TYPE_CHECK_INSTANCE_CAST((obj), CO_TYPE_PRINT, co_print_t)
#define CO_IS_PRINT(obj) G_TYPE_CHECK_INSTANCE_TYPE((obj), CO_TYPE_PRINT)
#define CO_PRINT_CLASS(kclass) (G_TYPE_CHECK_CLASS_CAST ((kclass), CO_TYPE_PRINT, co_print_tClass))
#define CO_IS_PRINT_CLASS(kclass) (G_TYPE_CHECK_CLASS_TYPE ((kclass), CO_TYPE_PRINT))
#define CO_PRINT_GET_CLASS(obj) (\
        G_TYPE_INSTANCE_GET_CLASS ((obj), CO_TYPE_PRINT, co_print_tClass))

typedef struct _co_print_t co_print_t;
typedef struct _co_print_tClass co_print_tClass;

struct _co_print_t
{
    GObject parent;
};

struct _co_print_tClass
{
    GObjectClass parent_class;
};

GType co_print_get_type (void);
void co_print_print1 (gpointer gp, gpointer udata);
void co_print_print2 (gpointer gp, gpointer udata);

#endif
