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
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Ding");
MODULE_DESCRIPTION("List all mounts in 5.4 via /proc/mounts");

#define MOUNTS_PATH "/proc/mounts"

ssize_t kernel_read_line(struct file* file, char* outBuf, size_t outLen, loff_t* pos)
{
    ssize_t ret;
    size_t total = 0;
    char c;

    if (outLen == 0)
        return -EINVAL;

    while (1) {
        ret = kernel_read(file, &c, 1, pos);
        if (ret <= 0)
            break;  // EOF or error

        if (c == '\n')
            break;

        if (total < outLen - 1) {
            outBuf[total++] = c;
        }
    }

    outBuf[total] = '\0';

    if (ret < 0)
        return ret;

    return total;
}

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
    while ((read_len = kernel_read_line(f, buf, 4095, &pos)) > 0) {
        int mpS = 0;
        int mpE = 0;
        buf[read_len] = '\0';
        if (read_len <= 5) {
            pr_err(">> %s", buf);
            continue;
        }
        if (buf[0] != '/' || buf[1] != 'd' || buf[2] != 'e' || buf[3] != 'v' || buf[4] != '/') {
            pr_err(">> %s", buf);
            continue;
        }

        mpS = 5;
        for (int i = mpS; i < read_len; i++) {
            if (mpS == 5 && (buf[i] == ' ' || buf[i] == '\t')) {
                mpS = i + 1;
                continue;
            }
            if (mpS > 5 && (buf[i] == ' ' || buf[i] == '\t')) {
                mpE = i;
                break;
            }
        }

        pr_info("==> %s", buf);
        // 获取挂载点
        if (mpE > mpS) {
            int i = 0;
            for (i = 0; i < mpE - mpS; ++i) {
                buf[i] = buf[mpS + i]; 
            }
            buf[i] = '\0';
            struct path path;
            int err = kern_path(buf, LOOKUP_FOLLOW, &path);
            if (0 != err) {
                pr_err("kern_path error => %s", buf);
                continue;
            }

            pr_info("==>MP %s, %s", buf, path.mnt->mnt_sb->s_type->name);
        }
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


