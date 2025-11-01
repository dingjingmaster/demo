/*************************************************************************
> FileName: ftrace3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月31日 星期五 17时08分44秒
 ************************************************************************/
// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Example");
MODULE_DESCRIPTION("Hook process exit via ftrace(do_exit)");
MODULE_VERSION("1.0");

/* ----------------- ftrace_hook 封装 ----------------- */
struct ftrace_hook {
    const char *name;            // 要 hook 的函数名
    void *function;              // hook 回调
    void *original;              // 保存原函数地址
    unsigned long address;
    struct ftrace_ops ops;
};

/* 根据符号名解析地址 */
static int fh_resolve_hook_address(struct ftrace_hook *hook)
{
    hook->address = kallsyms_lookup_name(hook->name);
    if (!hook->address) {
        pr_err("unresolved symbol: %s\n", hook->name);
        return -ENOENT;
    }
    if (hook->original)
        *((unsigned long *)hook->original) = hook->address;
    return 0;
}

/* ftrace thunk 回调，自动适配内核版本 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,11,0)
#define HOOK_FLAGS FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION
#else
#define HOOK_FLAGS FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION_SAFE
#endif

static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops, void *regs)
{
    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,11,0)
    struct ftrace_regs *fregs = regs;
    fregs->ip = (unsigned long)hook->function;
#else
    struct pt_regs *pregs = regs;
    pregs->pc = (unsigned long)hook->function;
#endif
}

/* 安装 hook */
int fh_install_hook(struct ftrace_hook *hook)
{
    int err;
    err = fh_resolve_hook_address(hook);
    if (err)
        return err;

    hook->ops.func = fh_ftrace_thunk;
    hook->ops.flags = HOOK_FLAGS;

    err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
    if (err)
        return err;

    err = register_ftrace_function(&hook->ops);
    if (err)
        ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);

    return err;
}

/* 移除 hook */
void fh_remove_hook(struct ftrace_hook *hook)
{
    unregister_ftrace_function(&hook->ops);
    ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
}

/* ----------------- hook 进程退出 ----------------- */
static void my_do_exit(unsigned long code)
{
    struct task_struct *task = current;
    pr_info("[exit_hook] pid=%d comm=%s exit_code=%lu parent_pid=%d\n",
            task->pid, task->comm, code,
            task->real_parent ? task->real_parent->pid : -1);

    /* 调用原始 do_exit */
    void (*orig_do_exit)(unsigned long) = (void *)kallsyms_lookup_name("do_exit");
    orig_do_exit(code);
}

/* 定义 ftrace_hook 对象 */
static struct ftrace_hook do_exit_hook = {
    .name = "do_exit",
    .function = my_do_exit,
    .original = NULL,
};

/* ----------------- 模块初始化/退出 ----------------- */
static int __init hook_init(void)
{
    int ret = fh_install_hook(&do_exit_hook);
    if (ret) {
        pr_err("hook do_exit failed: %d\n", ret);
        return ret;
    }
    pr_info("hook do_exit installed\n");
    return 0;
}

static void __exit hook_exit(void)
{
    fh_remove_hook(&do_exit_hook);
    pr_info("hook do_exit removed\n");
}

module_init(hook_init);
module_exit(hook_exit);
