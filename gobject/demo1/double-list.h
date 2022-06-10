/*************************************************************************
> FileName: double-list.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月03日 星期二 10时08分19秒
 ************************************************************************/
#ifndef _DOUBLE_LIST_H
#define _DOUBLE_LIST_H
#include <glib-object.h>

#define DEMO_TYPE_DLIST (demo_dlist_get_type())

typedef struct _demo_list_node_t demo_list_node_t;
typedef struct _demo_list_t demo_list_t;
typedef struct _demo_list_tClass demo_list_tClass;

struct _demo_list_node_t
{
    demo_list_node_t* prev;
    demo_list_node_t* next;
    void* data;
};

struct _demo_list_t
{
    GObject parent_instance;
    demo_list_node_t* head;
    demo_list_node_t* tail;
};

struct _demo_list_tClass
{
    GObjectClass parent_class;
};

// gobject 需要知道对象对应的类的数据结构，返回类型是 GType
GType demo_dlist_get_type (void);

#endif
