/*************************************************************************
> FileName: hlist.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 26 Oct 2023 03:03:15 PM CST
 ************************************************************************/
#include "hlist.h"

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/string.h>

/**
 *  用于实现哈希链表的 单链表 结构
 *  struct hlist_head {
 *      struct hlist_node* first;
 *  };
 *
 *  struct hlist_node {
 *      // pprev 保存的是前一个 hlist_node 的 next 成员的地址
 *      // 针对第一个元素，pprev 保存的是头节点 first 的地址
 *      struct hlist_node* next, **pprev; 
 *  }
 */

struct HlistNode
{
    char data[10];
    struct hlist_node list;
};


static void print_hlist (struct hlist_head* list);

void dj_demo_hlist(void)
{
    /* 初始化 */
    // 1.
    struct hlist_head list1 = HLIST_HEAD_INIT;

    // 2.
    struct hlist_head list2;
    INIT_HLIST_HEAD(&list2);

    // 3.
    HLIST_HEAD(list3);

    // 4.
    struct hlist_head* list4 = kzalloc (sizeof(*list4), GFP_KERNEL | __GFP_NOFAIL);
    INIT_HLIST_HEAD(list4);

    // 5.
    struct hlist_head* list5 = kmalloc (sizeof(*list5), GFP_KERNEL | __GFP_NOFAIL);
    memset(list5, 0xFF, sizeof (*list5));
    INIT_HLIST_HEAD(list5);

    /* 添加 */
    struct HlistNode a, b, c, d;

    a.data[0] = 'a'; a.data[1] = '\0';
    b.data[0] = 'b'; b.data[1] = '\0';
    c.data[0] = 'c'; c.data[1] = '\0';
    d.data[0] = 'd'; d.data[1] = '\0';

    hlist_add_head(&(a.list), &list1);
    hlist_add_head(&(b.list), &list1);
    hlist_add_before(&(c.list), &(a.list));
    hlist_add_behind(&(d.list), &(a.list));
    print_hlist(&list1);

    kfree (list4);
    kfree (list5);
}

static void print_hlist (struct hlist_head* list)
{
    printk("hlist==>");

    struct HlistNode*   curNode = NULL;
    struct hlist_node*  tmpNode = NULL;

    hlist_for_each_entry_safe (curNode, tmpNode, list, list) {
        printk("%s", curNode->data);
    }

    printk("hlist==<!");
}
