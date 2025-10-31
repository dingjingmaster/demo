/*************************************************************************
> FileName: ftrace1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月31日 星期五 17时00分33秒
 ************************************************************************/
// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/ftrace.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Example");
MODULE_DESCRIPTION("Trace process exit via ftrace(do_exit)");
MODULE_VERSION("1.0");

/* 要 hook 的目标函数 */
#define TARGET_SYM "do_exit"

/*
 * ftrace 回调函数签名在不同版本内核中不同：
 * < 5.11: void func(unsigned long ip, unsigned long parent_ip,
 *                   struct ftrace_ops *op, struct pt_regs *regs)
 * >=5.11: void func(unsigned long ip, unsigned long parent_ip,
 *                   struct ftrace_ops *op, struct ftrace_regs *fregs)
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,11,0)
static void notrace trace_do_exit(unsigned long ip, unsigned long parent_ip,
                                  struct ftrace_ops *op, struct ftrace_regs *fregs)
{
    struct task_struct *task = current;
    pr_info("[ftrace_exit] pid=%d comm=%s exit_code=%d\n",
            task->pid, task->comm, task->exit_code);
}
#else
static void notrace trace_do_exit(unsigned long ip, unsigned long parent_ip,
                                  struct ftrace_ops *op, struct pt_regs *regs)
{
    struct task_struct *task = current;
    pr_info("[ftrace_exit] pid=%d comm=%s exit_code=%d\n",
            task->pid, task->comm, task->exit_code);
}
#endif

static struct ftrace_ops my_ops __read_mostly = {
    .func = trace_do_exit,
    .flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION_SAFE,
};

static int __init my_ftrace_init(void)
{
    int ret;

    ret = ftrace_set_filter(&my_ops, TARGET_SYM, strlen(TARGET_SYM), 0);
    if (ret) {
        pr_err("ftrace_set_filter failed: %d\n", ret);
        return ret;
    }

    ret = register_ftrace_function(&my_ops);
    if (ret) {
        pr_err("register_ftrace_function failed: %d\n", ret);
        ftrace_set_filter(&my_ops, NULL, 0, 1); // 清除 filter
        return ret;
    }

    pr_info("ftrace hook installed on %s\n", TARGET_SYM);
    return 0;
}

static void __exit my_ftrace_exit(void)
{
    unregister_ftrace_function(&my_ops);
    ftrace_set_filter(&my_ops, NULL, 0, 1);
    pr_info("ftrace hook removed\n");
}

module_init(my_ftrace_init);
module_exit(my_ftrace_exit);
