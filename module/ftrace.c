/*************************************************************************
> FileName: ftrace.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月31日 星期五 16时56分33秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/ftrace.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

#define SYM_NAME "do_exit"

static void notrace my_exit_hook(unsigned long ip, unsigned long parent_ip,
                                 struct ftrace_ops *ops, struct ftrace_regs* regs)
{
    struct task_struct *task = current;
    pr_info("[ftrace_exit] pid=%d comm=%s exit_code=%d\n",
            task->pid, task->comm, task->exit_code);
}

static struct ftrace_ops my_ops = {
    .func = my_exit_hook,
    .flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION,
};

static int __init my_mod_init(void)
{
    int ret = ftrace_set_filter(&my_ops, SYM_NAME, strlen(SYM_NAME), 0);
    if (ret)
        return ret;
    ret = register_ftrace_function(&my_ops);
    if (ret)
        return ret;
    pr_info("ftrace hook on %s loaded\n", SYM_NAME);
    return 0;
}

static void __exit my_mod_exit(void)
{
    unregister_ftrace_function(&my_ops);
    pr_info("ftrace hook removed\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);
