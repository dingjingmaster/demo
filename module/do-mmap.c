/*************************************************************************
> FileName: do-mmap.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月10日 星期四 19时18分25秒
 ************************************************************************/

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/mm_types.h>
#include <uapi/linux/mman.h>
#include <uapi/asm-generic/mman-common.h>


static int target_pid = -1;
module_param(target_pid, int, 0);
MODULE_PARM_DESC(target_pid, "Target PID");
MODULE_LICENSE("GPL");

static int __init mmap_alloc_init(void)
{
    struct task_struct *task;
    struct mm_struct *mm;
    unsigned long len = PAGE_SIZE * 2;
    unsigned long addr = 0;
    unsigned long ret;

    if (target_pid <= 0) {
        pr_err("Please set a valid target_pid\n");
        return -EINVAL;
    }

    rcu_read_lock();
    task = pid_task(find_vpid(target_pid), PIDTYPE_PID);
    if (!task) {
        rcu_read_unlock();
        pr_err("Cannot find PID %d\n", target_pid);
        return -ESRCH;
    }

    mm = get_task_mm(task);
    rcu_read_unlock();
    if (!mm) {
        pr_err("Cannot get mm_struct\n");
        return -EINVAL;
    }

    //down_write(&mm->mmap_sem);
    down_write(&mm->mmap_lock);

    // 使用 do_mmap 分配匿名内存（不关联文件）
    ret = do_mmap(NULL, addr, len,
                  PROT_READ | PROT_WRITE,
                  MAP_ANONYMOUS | MAP_PRIVATE, 0, 0, NULL, NULL);

    //up_write(&mm->mmap_sem);
    up_write(&mm->mmap_lock);
    mmput(mm);

    if (IS_ERR_VALUE(ret)) {
        pr_err("do_mmap failed: %ld\n", ret);
        return -EFAULT;
    }

    pr_info("Successfully mmap'ed %lu bytes at 0x%lx for PID %d\n", len, ret, target_pid);

    return 0;
}

static void __exit mmap_alloc_exit(void)
{
    pr_info("mmap_alloc module unloaded.\n");
}

module_init(mmap_alloc_init);
module_exit(mmap_alloc_exit);
