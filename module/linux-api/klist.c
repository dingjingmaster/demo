/*************************************************************************
> FileName: ./klist.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 26 Oct 2023 03:59:45 PM CST
 ************************************************************************/
#include "klist.h"

#include <linux/slab.h>
#include <linux/list.h>
#include <linux/klist.h>
#include <linux/types.h>
#include <linux/string.h>

/**
 *  klist 提供了一种将内核对象组织成链表的机制，以方便对它们进行管理和操作
 *  每个 klist 都代表了一个特定类型的内核对象的链表，例如：文件、设备、驱动等
 *
 *  struct klist {
 *      spinlock_t          k_lock;     // 用于保护链表的自旋锁
 *      struct list_head    k_list;     // 内核对象链表的头节点
 *      int                 k_refount;  // 引用计数，用于跟踪对象的引用数目
 *      void(*get)(struct klist_node*); // 增加对象引用计数的函数指针
 *      void(*put)(struct klist_node*); // 减少对象引用计数的函数指针
 *  };
 *
 *  struct klist_node {
 *      void                *n_klist;   // 指向包含当前节点的 klist 
 *      struct list_head     n_node;    // 指向包含该节点的 klist 结构
 *      struct kref          n_ref;     // 用于跟踪节点的引用计数，确保在节点不再需要使用时候释放资源
 *  };
 *
 *  结合 struct klist 和 struct klist_node，可以创建一个链表结构，用于管理和操作内核对象。
 */

static int                  gNodeCount;
static struct klist_node*   gLastNode = NULL;

static void check_node (struct klist_node* nodePtr)
{
    ++gNodeCount;
    gLastNode = nodePtr;
}

static void check_delete_node (struct klist_node* nodePtr)
{
    --gNodeCount;
    gLastNode = nodePtr;
}

void dj_demo_klist(void)
{
    struct klist    list1;
    struct klist_node* curNode = NULL;

    gNodeCount = 0;
    klist_init(&list1, &check_node, NULL);

    struct klist_node a, b;
    klist_add_tail(&a, &list1);
    klist_add_tail(&b, &list1);

    printk("a: %p", &a);
    printk("b: %p", &b);

    struct klist_iter i;
    klist_iter_init(&list1, &i);
    for (curNode = klist_next(&i); curNode; curNode = klist_next(&i)) {
        printk("=: %p", curNode);
    }
    printk(" ");

    klist_iter_exit(&i);
}
