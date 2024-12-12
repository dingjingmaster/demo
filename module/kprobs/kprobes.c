#include <linux/fs.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/kprobes.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <generated/autoconf.h>

static struct kprobe kp;

int handler_pre(struct kprobe* p, struct pt_regs* regs)
{
    char __user* filename = (char __user*) regs->di;
    char buf[256];

    if (strncpy_from_user(buf, filename, sizeof(buf)) > 0) {
        printk(KERN_INFO "sys_open: %s\n", buf);
    }

    return 0;
}

void handler_post (struct kprobe* p, struct pt_regs* regs, unsigned long flags)
{
    printk(KERN_INFO "sys_open finished, with return value: %ld\n", regs->ax);
}


static int __init symbol_traverse_init(void)
{
    printk(KERN_INFO "Start symbol\n");

    kp.symbol_name = "do_sys_open";

    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;

    // 注册
    if (register_kprobe(&kp) < 0) {
        printk(KERN_ERR "Failed to register kprobe\n");
        return -1;
    }

    return 0;
}

static void __exit symbol_traverse_exit(void)
{
    printk(KERN_INFO "OK\n");
    unregister_kprobe(&kp);
}

module_init(symbol_traverse_init);
module_exit(symbol_traverse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DingJing");
