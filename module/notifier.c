/*************************************************************************
> FileName: notifier.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月31日 星期五 16时47分01秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/profile.h>   // profile_event_register, PROFILE_TASK_EXIT
#include <linux/sched/signal.h>

static int my_task_exit_notifier(struct notifier_block *self,
                                 unsigned long val, void *data)
{
    struct task_struct *task = data;

    pr_info("[task_exit_notifier] process exited: pid=%d comm=%s\n",
            task->pid, task->comm);

    return NOTIFY_OK;
}

static struct notifier_block my_nb = {
    .notifier_call = my_task_exit_notifier,
    .priority = 0,
};

static int __init my_mod_init(void)
{
    pr_info("task exit notifier module loaded\n");
    profile_event_register(PROFILE_TASK_EXIT, &my_nb);
    return 0;
}

static void __exit my_mod_exit(void)
{
    pr_info("task exit notifier module unloaded\n");
    profile_event_unregister(PROFILE_TASK_EXIT, &my_nb);
}

module_init(my_mod_init);
module_exit(my_mod_exit);
MODULE_LICENSE("GPL");
