/*************************************************************************
> FileName: work-queue.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月23日 星期四 09时36分16秒
 ************************************************************************/
// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/completion.h>
#include <linux/delay.h>

/* 用户任务函数签名：接收 void*，返回 int 作为“结果” */
typedef int (*tp_work_fn)(void *data);

/* 提交时返回的任务句柄 */
struct tp_task {
    struct work_struct work;
    struct completion done;
    tp_work_fn fn;
    void *data;
    int result;
    bool finished;
    /* 可扩展字段：err code, id, flags, timeout info 等 */
};

/* 线程池对象 */
struct tp_pool {
    struct workqueue_struct *wq;
    int max_workers;
};

static struct tp_pool *global_pool;

/* work wrapper：由 worker 线程执行 */
static void tp_work_wrapper(struct work_struct *work)
{
    struct tp_task *t = container_of(work, struct tp_task, work);

    if (!t || !t->fn) {
        t->result = -EINVAL;
        t->finished = true;
        complete(&t->done);
        return;
    }

    /* 执行用户函数（注意：内核上下文，必须非阻塞或可睡） */
    t->result = t->fn(t->data);
    t->finished = true;
    complete(&t->done);
}

/* 创建线程池，max_workers 为并发 worker 数 */
struct tp_pool *threadpool_create(const char *name, int max_workers)
{
    struct tp_pool *p;

    if (max_workers <= 0)
        return ERR_PTR(-EINVAL);

    p = kzalloc(sizeof(*p), GFP_KERNEL);
    if (!p)
        return ERR_PTR(-ENOMEM);

    /* flags: 0 使用默认，可加 WQ_UNBOUND/WQ_MEM_RECLAIM 等视需求 */
    p->wq = alloc_workqueue(name, 0, max_workers);
    if (!p->wq) {
        kfree(p);
        return ERR_PTR(-ENOMEM);
    }
    p->max_workers = max_workers;
    return p;
}

/* 销毁线程池：先 flush，再 destroy */
void threadpool_destroy(struct tp_pool *p)
{
    if (!p) return;
    flush_workqueue(p->wq);
    destroy_workqueue(p->wq);
    kfree(p);
}

/* 提交任务，返回任务句柄（caller 负责 wait & free） */
struct tp_task *threadpool_submit(struct tp_pool *p, tp_work_fn fn, void *data)
{
    struct tp_task *t;

    if (!p || !fn)
        return ERR_PTR(-EINVAL);

    t = kzalloc(sizeof(*t), GFP_KERNEL);
    if (!t)
        return ERR_PTR(-ENOMEM);

    t->fn = fn;
    t->data = data;
    init_completion(&t->done);
    t->finished = false;

    INIT_WORK(&t->work, tp_work_wrapper);

    if (!queue_work(p->wq, &t->work)) {
        /* queue_work 返回 false 当 work 已经在队列中 (不应发生) */
        kfree(t);
        return ERR_PTR(-EALREADY);
    }

    return t;
}

/* 等待任务完成并获取结果；timeout_ms < 0 表示无限等待 */
int threadpool_wait_result(struct tp_task *t, long timeout_ms, int *out_result)
{
    long ret;

    if (!t) return -EINVAL;

    if (timeout_ms < 0) {
        wait_for_completion(&t->done);
        ret = 0;
    } else {
        ret = wait_for_completion_timeout(&t->done, msecs_to_jiffies(timeout_ms));
        if (ret == 0) /* 超时 */
            return -ETIMEDOUT;
    }

    if (out_result)
        *out_result = t->result;

    /* 等待方负责释放任务结构（避免 worker 和等待者竞争 free） */
    kfree(t);
    return 0;
}

/* ------------ 简单示例：一个计算函数 -------------- */
static int example_fn(void *data)
{
    int x = (int)(long)data;
    /* 模拟工作（睡眠仅示例；实际内核函数不应长时间睡） */
    msleep(100);
    return x * x;
}

/* 模块初始化：创建池并提交若干任务示例 */
static int __init tp_mod_init(void)
{
    int i;
    struct tp_task *t;
    int res;

    pr_info("tp_mod: init\n");
    global_pool = threadpool_create("my_tp", 4);
    if (IS_ERR(global_pool)) {
        pr_err("tp_mod: create pool failed\n");
        return PTR_ERR(global_pool);
    }

    /* 提交 8 个任务，示例等待并打印结果 */
    for (i = 0; i < 8; i++) {
        t = threadpool_submit(global_pool, example_fn, (void *)(long)i);
        if (IS_ERR(t)) {
            pr_err("submit failed\n");
            continue;
        }
        res = threadpool_wait_result(t, 5000 /*ms*/, &res);
        if (res == -ETIMEDOUT)
            pr_warn("task %d timed out\n", i);
        else
            pr_info("task %d result %d\n", i, res);
    }

    return 0;
}

static void __exit tp_mod_exit(void)
{
    pr_info("tp_mod: exit\n");
    threadpool_destroy(global_pool);
    global_pool = NULL;
}

module_init(tp_mod_init);
module_exit(tp_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Example");
MODULE_DESCRIPTION("Simple kernel threadpool example with waitable tasks");
