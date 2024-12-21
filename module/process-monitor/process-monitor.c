#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>

static int process_fork_event(struct notifier_block* self, unsigned long val, void* data)
{
    struct task_struct* task = data;
    printk(KERN_INFO "New process: Pid=%d, Name=%s\n", task->pid, task->comm);

    return NOTIFY_OK;
}

static struct notifier_block process_notifier = {
    .notifier_call = process_fork_event,
};

static int __init symbol_traverse_init(void)
{
    printk(KERN_INFO "Start symbol\n");

    register_sched_notifier(&process_notifier);

    return 0;
}

static void __exit symbol_traverse_exit(void)
{
    printk(KERN_INFO "OK\n");
    unregister_sched_notifier(&process_notifier);
}

module_init(symbol_traverse_init);
module_exit(symbol_traverse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DingJing");
