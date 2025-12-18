#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Hook openat on ARM64");

static struct kprobe kp = {
    .symbol_name = "do_sys_openat2",  // 5.4 内核中实际处理函数
};

/* ARM64 寄存器约定:
 * x0 = dfd (dirfd)
 * x1 = filename
 * x2 = flags
 * x3 = mode
 */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    char __user *filename = (char __user *)regs->regs[1];
    char buf[256];
    
    if (strncpy_from_user(buf, filename, sizeof(buf) - 1) > 0) {
        buf[sizeof(buf) - 1] = '\0';
        pr_info("openat: pid=%d, comm=%s, file=%s\n",
                current->pid, current->comm, buf);
    }
    
    return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs,
                         unsigned long flags)
{
    /* x0 包含返回值 (fd 或错误码) */
    pr_info("openat returned: %ld\n", regs->regs[0]);
}

static int __init hook_init(void)
{
    int ret;
    
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    
    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    
    pr_info("kprobe registered at %pS\n", kp.addr);
    return 0;
}

static void __exit hook_exit(void)
{
    unregister_kprobe(&kp);
    pr_info("kprobe unregistered\n");
}

module_init(hook_init);
module_exit(hook_exit);
