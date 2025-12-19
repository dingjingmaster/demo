#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

/* ========= per-task 递归保护 ========= */

/*
 * 利用 task_struct->flags 中的一个自定义 bit
 * PF_FREEZER_SKIP 在厂商内核一般是空闲的
 */
#define PF_OPEN_REDIRECT 0x10000000

/* ========= fdtable 关闭 fd ========= */

static void force_close_fd(int fd)
{
    struct files_struct *files = current->files;
    struct file *file;

    spin_lock(&files->file_lock);
    file = fcheck_files(files, fd);
    if (file)
        __close_fd(files, fd);
    spin_unlock(&files->file_lock);
}

/* ========= VFS 打开 ========= */

static int vfs_open_file(const char *path, int flags, umode_t mode)
{
    struct file *filp;
    int fd;

    filp = filp_open(path, flags, mode);
    if (IS_ERR(filp))
        return PTR_ERR(filp);

    fd = get_unused_fd_flags(flags);
    if (fd < 0) {
        fput(filp);
        return fd;
    }

    fd_install(fd, filp);
    return fd;
}

/* ========= kretprobe ========= */

static struct kretprobe rp;

static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
#if defined(CONFIG_ARM64)
    int fd = regs->regs[0];
#else
    int fd = regs->ax;
#endif

    /* open 失败 or 已在重定向中 → 跳过 */
    if (fd < 0)
        return 0;

    if (current->flags & PF_OPEN_REDIRECT)
        return 0;

    /* 标记：进入重定向 */
    current->flags |= PF_OPEN_REDIRECT;

    /* 关闭原 fd */
    force_close_fd(fd);

    /* 打开替代文件 */
    fd = vfs_open_file("/tmp/passwd.fake", O_RDONLY, 0);

#if defined(CONFIG_ARM64)
    regs->regs[0] = fd;
#else
    regs->ax = fd;
#endif

    /* 清除标记 */
    current->flags &= ~PF_OPEN_REDIRECT;

    return 0;
}

/* ========= 模块加载 ========= */

static int __init mod_init(void)
{
    rp.kp.symbol_name = "__se_sys_openat";
    rp.handler = ret_handler;
    rp.maxactive = 64;

    return register_kretprobe(&rp);
}

static void __exit mod_exit(void)
{
    unregister_kretprobe(&rp);
}

module_init(mod_init);
module_exit(mod_exit);
