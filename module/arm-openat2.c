#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");

#define HOOK_FUNC "do_sys_openat2"

static unsigned long hook_addr;

/* 用于保存原始函数指针 */
static void (*orig_func)(void);

static struct ftrace_ops ops;

static void notrace ftrace_callback(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *op,
                                    struct ftrace_regs *fregs)
{
    struct pt_regs *regs = ftrace_get_regs(fregs);
    char __user *filename;
    char buf[256];
    
    if (!regs)
        return;
    
    /* ARM64: x1 = filename 参数 */
    filename = (char __user *)regs->regs[1];
    
    if (strncpy_from_user(buf, filename, sizeof(buf) - 1) > 0) {
        buf[sizeof(buf) - 1] = '\0';
        pr_info("ftrace hook: pid=%d, comm=%s, file=%s\n",
                current->pid, current->comm, buf);
    }
}

static int __init hook_init(void)
{
    int ret;
    
    hook_addr = kallsyms_lookup_name(HOOK_FUNC);
    if (!hook_addr) {
        pr_err("Could not find %s\n", HOOK_FUNC);
        return -ENOENT;
    }
    
    pr_info("Found %s at %lx\n", HOOK_FUNC, hook_addr);
    
    ops.func = ftrace_callback;
    ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY;
    
    ret = ftrace_set_filter_ip(&ops, hook_addr, 0, 0);
    if (ret) {
        pr_err("ftrace_set_filter_ip failed: %d\n", ret);
        return ret;
    }
    
    ret = register_ftrace_function(&ops);
    if (ret) {
        pr_err("register_ftrace_function failed: %d\n", ret);
        ftrace_set_filter_ip(&ops, hook_addr, 1, 0);
        return ret;
    }
    
    pr_info("ftrace hook installed\n");
    return 0;
}

static void __exit hook_exit(void)
{
    unregister_ftrace_function(&ops);
    ftrace_set_filter_ip(&ops, hook_addr, 1, 0);
    pr_info("ftrace hook removed\n");
}

module_init(hook_init);
module_exit(hook_exit);
