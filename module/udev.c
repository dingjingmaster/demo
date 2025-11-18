/*************************************************************************
> FileName: udev.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月18日 星期二 11时01分20秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>

static int block_event_notify(struct notifier_block *self,
                              unsigned long action, void *dev)
{
    struct gendisk *gd = dev;

    switch (action) {
        case GENHD_ADD:
            pr_info("Block device added: %s\n", gd->disk_name);
            break;
        case GENHD_REMOVE:
            pr_info("Block device removed: %s\n", gd->disk_name);
            break;
    }
    return NOTIFY_OK;
}

static struct notifier_block block_nb = {
    .notifier_call = block_event_notify,
    .priority = 0,
};

static int __init block_monitor_init(void)
{
    register_disk_notifier(&block_nb);
    pr_info("Block device monitor loaded\n");
    return 0;
}

static void __exit block_monitor_exit(void)
{
    unregister_disk_notifier(&block_nb);
    pr_info("Block device monitor unloaded\n");
}

module_init(block_monitor_init);
module_exit(block_monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex");
MODULE_DESCRIPTION("Block device add/remove monitor");
