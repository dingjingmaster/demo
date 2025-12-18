#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChatGPT");
MODULE_DESCRIPTION("kprobe hook openat on ARM64");
MODULE_VERSION("1.0");

/* kprobe 对象 */
static struct kprobe kp;

/* pre_handler: openat 函数入口 */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    char fname[256] = {0};

#if defined(CONFIG_ARM64)
    const char __user *user_fname = (const char __user *)regs->regs[1];
#else
    const char __user *user_fname = (const char __user *)regs->di;
#endif

    if (user_fname)
        copy_from_user(fname, user_fname, sizeof(fname) - 1);

    printk(KERN_INFO "[kprobe openat] pid=%d comm=%s file=%s\n",
           current->pid, current->comm, fname);

    return 0; // 继续执行原函数
}

/* 初始化模块 */
static int __init kprobe_init(void)
{
    int ret;

    kp.symbol_name = "__se_sys_openat"; // 对应 ARM64 内核
    kp.pre_handler = handler_pre;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_ERR "register_kprobe failed: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "kprobe openat registered\n");
    return 0;
}

/* 卸载模块 */
static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);
    printk(KERN_INFO "kprobe openat unregistered\n");
}

module_init(kprobe_init);
module_exit(kprobe_exit);
