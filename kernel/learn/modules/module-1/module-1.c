/*************************************************************************
> FileName: module-1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 01 Dec 2023 10:14:27 AM CST
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("MIT");

static int __init __module_address_init(void);
static void __exit __module_address_exit(void);

int a_module(void)
{
    return 0;
}

int __init __module_address_init(void)
{
    struct module* ret = NULL;
    unsigned long addr = (unsigned long) a_module;

    preempt_disable();  // 禁止抢占
    ret = __module_address(addr);
    preempt_enable();   // 允许抢占
    if (NULL != ret) {
        printk (KERN_WARNING "ret->name: %s\n", ret->name); // 模块名称
        printk (KERN_WARNING "ret->state: %d\n", ret->state); 
        printk (KERN_WARNING "ref: %d\n", module_refcount(ret)); 
    }
    else {
        printk (KERN_WARNING "module_address return NULL\n");
    }

    return 0;
}

void __exit __module_address_exit(void)
{
    printk (KERN_WARNING "exit\n");
}

module_init(__module_address_init);
module_exit(__module_address_exit);
