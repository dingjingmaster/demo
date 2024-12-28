/*************************************************************************
> FileName: hello-world.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年05月11日 星期一 10时03分37秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/init.h>

static int __init init_hello(void)
{
    printk(KERN_ALERT "my first module test,output \"hello!\"\n");
    return 0;
}

static void __exit exit_hello(void)
{
    printk(KERN_DEBUG "bye bye test module!\n");
}

module_init(init_hello);
module_exit(exit_hello);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ding Jing");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("MODULE DEMO");
