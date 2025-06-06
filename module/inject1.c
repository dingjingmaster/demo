/**
 * 内核模块：用于拦截对 /etc/ld.preload.so 的读写操作
 * 功能：当 cat 进程启动时，注入 libtest.so
 */

// 必要的内核头文件
#include <linux/module.h>    // 模块相关定义
#include <linux/kernel.h>    // 内核相关定义
#include <linux/init.h>      // 模块初始化和清理函数
#include <linux/fs.h>        // 文件系统相关定义
#include <linux/namei.h>     // 路径名查找相关定义
#include <linux/version.h>   // 内核版本信息
#include <linux/cred.h>      // 进程凭证相关定义
#include <linux/security.h>  // 安全模块相关定义
#include <linux/path.h>      // 路径相关定义
#include <linux/string.h>    // 字符串操作函数
#include <linux/dcache.h>    // 目录缓存相关定义
#include <linux/lsm_hooks.h> // LSM钩子函数定义
#include <linux/sched.h>     // 进程调度相关定义
#include <linux/uaccess.h>   // 用户空间访问函数
#include <linux/file.h>      // 文件操作相关定义
#include <linux/kprobes.h>   // kprobe 相关定义
#include <linux/atomic.h>    // 原子操作相关定义

// 模块信息
MODULE_LICENSE("GPL");           // 模块许可证
MODULE_AUTHOR("Your Name");      // 模块作者
MODULE_DESCRIPTION("A kernel module for file injection"); // 模块描述
MODULE_VERSION("0.1");           // 模块版本

// 原始函数指针
static asmlinkage ssize_t (*original_vfs_read)(struct file *file, char __user *buf, size_t count, loff_t *pos);
static int (*original_vfs_getattr)(const struct path *path, struct kstat *stat, u32 request_mask, unsigned int query_flags);

// 钩子标志
static atomic_t hook_enabled = ATOMIC_INIT(0);

// kprobe 结构
static struct kprobe kp_read = {
    .symbol_name = "vfs_read",
};

static struct kprobe kp_getattr = {
    .symbol_name = "vfs_getattr",
};

// 目标文件路径
static const char *target_file = "/etc/ld.preload.so";

// 获取进程对应的注入库路径
static const char *get_process_lib(const char *comm)
{
    return "/path/to/inject/lib.so";
}

// 目标进程列表
static const char *target_processes[] = {
    "cat",
    NULL
};

// 检查是否是目标进程
static bool is_target_process(const char *comm)
{
    int i;
    for (i = 0; target_processes[i] != NULL; i++) {
        if (strcmp(comm, target_processes[i]) == 0) {
            return true;
        }
    }
    return false;
}

// kprobe 前置处理函数
static int handler_pre_read(struct kprobe *p, struct pt_regs *regs)
{
    struct file *file = (struct file *)regs->di;
    char __user *buf = (char __user *)regs->si;
    size_t count = (size_t)regs->dx;
    loff_t *pos = (loff_t *)regs->r10;
    char comm[TASK_COMM_LEN];
    const char *lib_path;
    size_t len;

    // 如果钩子未启用，直接返回
    if (!atomic_read(&hook_enabled))
        return 0;

    // 获取当前进程名称
    get_task_comm(comm, current);
    printk(KERN_INFO "vfs_read: comm=%s, file=%s\n", comm, file->f_path.dentry->d_name.name);

    // 检查是否是目标进程
    if (!is_target_process(comm))
        return 0;

    // 检查是否是目标文件
    if (strcmp(file->f_path.dentry->d_name.name, "ld.preload.so") != 0)
        return 0;

    // 获取注入库路径
    lib_path = get_process_lib(comm);
    if (!lib_path)
        return 0;

    // 计算路径长度
    len = strlen(lib_path);
    if (len > count)
        len = count;

    // 复制路径到用户空间
    if (copy_to_user(buf, lib_path, len)) {
        regs->ax = -EFAULT;
        return 1;
    }

    regs->ax = len;
    return 1;
}

static int handler_pre_getattr(struct kprobe *p, struct pt_regs *regs)
{
    const struct path *path = (const struct path *)regs->di;
    struct kstat *stat = (struct kstat *)regs->si;
    u32 request_mask = (u32)regs->dx;
    unsigned int query_flags = (unsigned int)regs->cx;
    char comm[TASK_COMM_LEN];
    struct timespec64 now;

    // 如果钩子未启用，直接返回
    if (!atomic_read(&hook_enabled))
        return 0;

    // 获取当前进程名称
    get_task_comm(comm, current);
    printk(KERN_INFO "vfs_getattr: comm=%s, file=%s\n", comm, path->dentry->d_name.name);

    // 检查是否是目标进程
    if (!is_target_process(comm))
        return 0;

    // 检查是否是目标文件
    if (strcmp(path->dentry->d_name.name, "ld.preload.so") != 0)
        return 0;

    // 获取当前时间
    ktime_get_real_ts64(&now);

    // 设置文件属性
    stat->mode = S_IFREG | 0644;
    stat->nlink = 1;
    stat->size = 4096;
    stat->blocks = 8;
    stat->blksize = 4096;
    stat->rdev = 0;
    stat->attributes = 0;
    stat->attributes_mask = 0;
    stat->ino = 1;
    stat->dev = 0;
    stat->uid = current_uid();
    stat->gid = current_gid();
    stat->atime = now;
    stat->mtime = now;
    stat->ctime = now;

    regs->ax = 0;
    return 1;
}

// 模块初始化函数
static int __init inject_init(void)
{
    int ret;

    // 注册 vfs_read kprobe
    kp_read.pre_handler = handler_pre_read;
    kp_read.post_handler = NULL;
    ret = register_kprobe(&kp_read);
    if (ret < 0) {
        printk(KERN_ERR "register_kprobe for vfs_read failed, returned %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "vfs_read kprobe registered at %p\n", kp_read.addr);

    // 注册 vfs_getattr kprobe
    kp_getattr.pre_handler = handler_pre_getattr;
    kp_getattr.post_handler = NULL;
    ret = register_kprobe(&kp_getattr);
    if (ret < 0) {
        unregister_kprobe(&kp_read);
        printk(KERN_ERR "register_kprobe for vfs_getattr failed, returned %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "vfs_getattr kprobe registered at %p\n", kp_getattr.addr);

    // 启用钩子
    atomic_set(&hook_enabled, 1);

    printk(KERN_INFO "Inject module loaded\n");
    return 0;
}

// 模块清理函数
static void __exit inject_exit(void)
{
    // 禁用钩子
    atomic_set(&hook_enabled, 0);

    // 注销 kprobe
    unregister_kprobe(&kp_read);
    unregister_kprobe(&kp_getattr);
    printk(KERN_INFO "Inject module unloaded\n");
}

module_init(inject_init);
module_exit(inject_exit); 