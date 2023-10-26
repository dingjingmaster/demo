/*************************************************************************
> FileName: list-management.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 26 Oct 2023 08:53:14 AM CST
 ************************************************************************/
#include "list-management.h"

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/string.h>

/**
 * #include <linux/types.h>
 * struct list_head {
 *      struct list_head *next, *prev;
 * };
 */

typedef struct ListNode     ListNode;

struct ListNode
{
    struct list_head list;
    char data[10];
};

static void print_list (struct list_head* list);

void dj_demo_list(void)
{
    /* 初始化 */
    // 1.
    // 可以使用,
    struct list_head list1 = LIST_HEAD_INIT(list1);

    // 2.
    struct list_head list2;
    INIT_LIST_HEAD(&list2);

    // 3.
    LIST_HEAD(list3);

    // 4.
    struct list_head* list4 = kzalloc (sizeof(*list4), GFP_KERNEL | __GFP_NOFAIL);
    INIT_LIST_HEAD(list4);

    // 5.
    struct list_head* list5 = kmalloc (sizeof(*list5), GFP_KERNEL | __GFP_NOFAIL);
    memset(list5, 0xFF, sizeof(*list5));
    INIT_LIST_HEAD(list5);

    /* 添加 链表头 */
    ListNode a, b;
    a.data[0] = 'a';
    a.data[1] = '\0';

    b.data[0] = 'b';
    b.data[1] = '\0';
    list_add(&(a.list), &list1);        // 头插
    list_add(&(b.list), &list1);

    ListNode c, d;
    c.data[0] = 'c';
    c.data[1] = '\0';

    d.data[0] = 'd';
    d.data[1] = '\0';
    list_add_tail(&(c.list), &list1);   // 尾插
    list_add_tail(&(d.list), &list1);
    printk ("b -> a -> c -> d");
    print_list(&list1);
    // 此处应该是 list1 -> a -> b -> c -> d

    /* 删除 */
    // list_del_init(&a);
    // list_del_init_careful(&a);
    list_del(&(a.list));
    printk ("b -> c -> d");
    print_list(&list1);
    // 此处应该是 list1 -> b -> c -> d

    /* 替换 */
    // 1.
    ListNode e;
    e.data[0] = 'e';
    e.data[1] = '\0';
    list_replace(&(b.list), &(e.list)); 
    printk ("e -> c -> d");
    print_list(&list1);
    // 此处应该是 list1 -> e -> c -> d

    // 2.
    ListNode f;
    f.data[0] = 'f';
    f.data[1] = '\0';
    list_replace_init(&(d.list), &(f.list));
    printk ("e -> c -> f");
    print_list(&list1);
    // 此处应该是 list1 -> e -> c -> f

    /* 交换 */
    list_swap(&(e.list), &(c.list));
    printk ("c -> e -> f");
    print_list(&list1);
    // 此处应该是 list1 -> c -> e -> f

    /* 移动 */
    // 节点从一个链表移动到另一个链表
    // list_move_tail
    list_move(&(e.list), &list2);
    printk ("c -> f");
    print_list(&list1);
    // list1 -> c -> f
    // list2 -> e

    /* empty */
    // 1. empty
    list_empty(&list1);
    printk ("empty");
    print_list(&list1);

    // 2. epmty_careful
    list_empty_careful(&list1);
    list_empty_careful(&list2);

    /* rotate */
    INIT_LIST_HEAD(&list1);
    // `list_add_tail` list add node after specified head, This is good for implementing stacks
    list_add_tail(&(a.list), &list1);
    list_add_tail(&(b.list), &list1);
    list_add_tail(&(c.list), &list1);
    list_add_tail(&(d.list), &list1);
    list_add_tail(&(e.list), &list1);
    list_add_tail(&(f.list), &list1);
    //list_add(&(a.list), &list1);
    //list_add(&(b.list), &list1);
    //list_add(&(c.list), &list1);
    printk ("a -> b -> c -> d -> e -> f");
    print_list(&list1);

    // 1. rotate left
    // 类似循环左移，左边移出的补到右边
    list_rotate_left(&list1);
    printk("list_rotate_left: a -> b -> c -> d -> e -> f");
    print_list(&list1);
    list_rotate_left(&list1);
    printk("list_rotate_left: b -> c -> d -> e -> f -> a");
    print_list(&list1);
    list_empty_careful(&list1);

    // 2. rotate front
    INIT_LIST_HEAD(&list1);
    list_add_tail(&(a.list), &list1);
    list_add_tail(&(b.list), &list1);
    list_add_tail(&(c.list), &list1);
    list_add_tail(&(d.list), &list1);
    list_rotate_to_front(&(c.list), &list1);
    printk("list_rotate_front: a -> b -> c -> d");
    print_list(&list1);         // 结果是：c -> d -> b -> a
    list_empty_careful(&list1);

    /* is singular */
    // 是不是只有一个节点
    INIT_LIST_HEAD(&list1);
    list_add_tail(&(a.list), &list1);
    printk("is singular: %s", (list_is_singular(&list1) ? "true" : "false"));

    INIT_LIST_HEAD(&list1);
    printk("is singular: %s", (list_is_singular(&list1) ? "true" : "false"));

    /* cut */
    // 将一个链表切为两个


    /* 释放 */
    kfree(list4);
    kfree(list5);
}

static void print_list (struct list_head* list)
{
    printk ("==>");

    struct list_head* cur = NULL;
    // NOTE:// list_for_each 第二个参数传入的是 list 头，不是链表的第一个元素
    list_for_each (cur, list) {
        ListNode* curNode = container_of(cur, ListNode, list);
        printk(KERN_INFO " %s\n", (const char*) (curNode->data));
    }
    printk("===<!");
}
