/*************************************************************************
> FileName: rht-rcu.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月31日 星期五 13时15分32秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/rhashtable.h>
#include <linux/rcupdate.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Example");
MODULE_DESCRIPTION("RCU-safe rhashtable demo (integer keys)");
MODULE_VERSION("0.1");

#define DEMO_ENTRIES 5

struct demo_entry {
    u32 id;                 /* key */
    int value;
    struct rhash_head node; /* rhashtable linkage */
};

static struct rhashtable demo_ht;
static atomic_t stop_thread;
static struct task_struct *reader_thread;

/* rhashtable 参数 */
static const struct rhashtable_params demo_params = {
    .key_len = sizeof(u32),
    .key_offset = offsetof(struct demo_entry, id),
    .head_offset = offsetof(struct demo_entry, node),
    .automatic_shrinking = true,
};

/* 插入新元素 */
static int demo_insert(u32 id, int val)
{
    struct demo_entry *e;

    e = kmalloc(sizeof(*e), GFP_KERNEL);
    if (!e)
        return -ENOMEM;

    e->id = id;
    e->value = val;

    if (rhashtable_insert_fast(&demo_ht, &e->node, demo_params)) {
        pr_err("rht: insert failed for id=%u\n", id);
        kfree(e);
        return -EEXIST;
    }

    pr_info("rht: inserted id=%u val=%d\n", id, val);
    return 0;
}

/* RCU 读查找 */
static struct demo_entry *demo_lookup_rcu(u32 id)
{
    struct demo_entry *e;
    rcu_read_lock();
    e = rhashtable_lookup_fast(&demo_ht, &id, demo_params);
    rcu_read_unlock();
    return e;
}

/* 删除元素 */
static int demo_remove(u32 id)
{
    struct demo_entry *e;

    rcu_read_lock();
    e = rhashtable_lookup_fast(&demo_ht, &id, demo_params);
    rcu_read_unlock();

    if (!e)
        return -ENOENT;

    rhashtable_remove_fast(&demo_ht, &e->node, demo_params);

    /* 等待所有 RCU 读者退出 */
    synchronize_rcu();

    pr_info("rht: removed id=%u val=%d\n", id, e->value);
    kfree(e);
    return 0;
}

/* 模拟读者线程 */
static int demo_reader_fn(void *arg)
{
    u32 ids[] = {1, 2, 3, 4, 5};
    size_t i;

    while (!kthread_should_stop() && !atomic_read(&stop_thread)) {
        for (i = 0; i < ARRAY_SIZE(ids); i++) {
            struct demo_entry *e;

            rcu_read_lock();
            e = rhashtable_lookup_fast(&demo_ht, &ids[i], demo_params);
            if (e)
                pr_info("rht(reader): found id=%u val=%d\n", e->id, e->value);
            else
                pr_info("rht(reader): id=%u not found\n", ids[i]);
            rcu_read_unlock();

            msleep(500);
            if (kthread_should_stop() || atomic_read(&stop_thread))
                break;
        }
        msleep(500);
    }
    pr_info("rht: reader thread exiting\n");
    return 0;
}

/* 模块初始化 */
static int __init rht_rcu_demo_init(void)
{
    int i, ret;

    pr_info("rht: init\n");

    ret = rhashtable_init(&demo_ht, &demo_params);
    if (ret) {
        pr_err("rht: rhashtable_init failed: %d\n", ret);
        return ret;
    }

    /* 插入元素 */
    for (i = 0; i < DEMO_ENTRIES; i++) {
        ret = demo_insert(i + 1, (i + 1) * 10);
        if (ret)
            pr_warn("rht: insert %d failed: %d\n", i + 1, ret);
    }

    /* 启动读者线程 */
    atomic_set(&stop_thread, 0);
    reader_thread = kthread_run(demo_reader_fn, NULL, "rht_reader");
    if (IS_ERR(reader_thread)) {
        pr_err("rht: failed to start reader thread\n");
        rhashtable_destroy(&demo_ht);
        return PTR_ERR(reader_thread);
    }

    return 0;
}

/* 模块退出 */
static void __exit rht_rcu_demo_exit(void)
{
    int i;
    struct demo_entry *e;
    struct rhash_head *tmp;

    pr_info("rht: exit\n");

    /* 停止线程 */
    atomic_set(&stop_thread, 1);
    if (reader_thread)
        kthread_stop(reader_thread);

    /* 删除所有元素 */
    for (i = 1; i <= DEMO_ENTRIES; i++) {
        demo_remove(i);
    }

    rhashtable_destroy(&demo_ht);
}

module_init(rht_rcu_demo_init);
module_exit(rht_rcu_demo_exit);
