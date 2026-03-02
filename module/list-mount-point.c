/*************************************************************************
> FileName: list-mount-point.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年03月02日 星期一 10时12分28秒
 ************************************************************************/
// 文件名: list_mounts_proc.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Ding");
MODULE_DESCRIPTION("List all mounts in 5.4 via /proc/mounts");

#define MOUNTS_PATH "/proc/mounts"

static int __init list_mounts_init(void)
{
    struct file *f;
    mm_segment_t old_fs;
    char *buf;
    loff_t pos = 0;
    ssize_t read_len;

    buf = kmalloc(4096, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    // 打开 /proc/mounts
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    f = filp_open(MOUNTS_PATH, O_RDONLY, 0);
    if (IS_ERR(f)) {
        pr_err("Failed to open %s\n", MOUNTS_PATH);
        set_fs(old_fs);
        kfree(buf);
        return PTR_ERR(f);
    }

    pr_info("=== Listing all mounts from %s ===\n", MOUNTS_PATH);

    // 读取文件内容
    while ((read_len = kernel_read(f, buf, 4095, &pos)) > 0) {
        buf[read_len] = '\0';
        pr_info("%s", buf);
    }

    filp_close(f, NULL);
    set_fs(old_fs);
    kfree(buf);

    return 0;
}

static void __exit list_mounts_exit(void)
{
    pr_info("=== Module unloaded ===\n");
}

module_init(list_mounts_init);
module_exit(list_mounts_exit);
