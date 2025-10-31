/*************************************************************************
> FileName: notifier1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月31日 星期五 16时50分08秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <trace/events/sched.h>  // for tracepoints

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Example");
MODULE_DESCRIPTION("Trace process exit events");

static void my_sched_process_exit(void *ignore, struct task_struct *p)
{
    pr_info("[trace_exit] pid=%d comm=%s exit_code=%d\n", p->pid, p->comm, p->exit_code);
}

static int __init my_mod_init(void)
{
    int ret;
    ret = register_trace_sched_process_exit(my_sched_process_exit, NULL);
    if (ret) {
        pr_err("Failed to register sched_process_exit tracepoint\n");
        return ret;
    }
    pr_info("sched_process_exit trace registered\n");
    return 0;
}

static void __exit my_mod_exit(void)
{
    unregister_trace_sched_process_exit(my_sched_process_exit, NULL);
    pr_info("sched_process_exit trace unregistered\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);

