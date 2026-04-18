#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/unistd.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/linkage.h>
#include <linux/limits.h>
#include <linux/vmalloc.h>
#include <linux/preempt.h>
#include <linux/ratelimit.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/ptrace.h>
#include <asm/page.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ARM64 syscall hook - Hardware-Assisted Patching (Kernel 4.19)");

#ifndef __NR_renameat
#define __NR_renameat 38
#endif

#ifndef __NR_renameat2
#define __NR_renameat2 276
#endif

static unsigned long *sys_call_table_addr = NULL;
static unsigned long *mapped_table = NULL;
static unsigned long mapped_base_va = 0;
typedef asmlinkage long (*sys_renameat_t)(int olddfd, const char __user *oldname, int newdfd, const char __user *newname);
typedef asmlinkage long (*sys_renameat2_t)(int olddfd, const char __user *oldname, int newdfd, const char __user *newname, unsigned int flags);
typedef asmlinkage long (*sys_renameat_regs_t)(const struct pt_regs *regs);
typedef asmlinkage long (*sys_renameat2_regs_t)(const struct pt_regs *regs);
static sys_renameat_regs_t orig_renameat = NULL;
static sys_renameat2_regs_t orig_renameat2 = NULL;
static DEFINE_SPINLOCK(hook_lock);
enum { HOOK_PATH_LOG_MAX = 1024, HOOK_PATH_PROBE_MAX = 4096 };

static void copy_user_path(char *dst, size_t dst_size, const char __user *upath)
{
    size_t ulen;
    long n;
    static const char suffix[] = "...(truncated)";
    size_t suffix_len = sizeof(suffix) - 1;
    size_t pos;

    if (!dst || dst_size == 0)
        return;
    dst[0] = '\0';

    if (!upath) {
        strscpy(dst, "<null>", dst_size);
        return;
    }

    ulen = strnlen_user(upath, HOOK_PATH_PROBE_MAX);
    if (ulen == 0) {
        strscpy(dst, "<fault>", dst_size);
        return;
    }
    if (ulen == 1) {
        strscpy(dst, "<empty>", dst_size);
        return;
    }

    n = strncpy_from_user(dst, upath, dst_size - 1);
    if (n < 0) {
        snprintf(dst, dst_size, "<fault:%ld>", n);
        return;
    }
    dst[dst_size - 1] = '\0';

    if (ulen > dst_size) {
        pos = (dst_size - 1 > suffix_len) ? (dst_size - 1 - suffix_len) : 0;
        memcpy(dst + pos, suffix, min(suffix_len, dst_size - 1 - pos));
        dst[dst_size - 1] = '\0';
    }
}

static void get_syscall_args(const struct pt_regs *regs, unsigned long *args, unsigned int n)
{
    unsigned int i;

    if (!regs || !args || n == 0)
        return;

#ifdef CONFIG_ARM64
    if (n > 6)
        n = 6;
    for (i = 0; i < n; i++)
        args[i] = regs->regs[i];
#else
    for (i = 0; i < n; i++)
        args[i] = 0;
    pr_warn_ratelimited("renameat_hook: get_syscall_args fallback path in non-ARM64 build\n");
#endif
}

static int init_table_mapping(void)
{
    struct page **pages;
    unsigned long start_va, end_va;
    int nr_pages;
    int i;

    start_va = ((unsigned long)sys_call_table_addr) & PAGE_MASK;
    end_va = ((unsigned long)(sys_call_table_addr + __NR_renameat2 + 1)) & PAGE_MASK;
    nr_pages = (end_va - start_va) / PAGE_SIZE + 1;
    mapped_base_va = start_va;

    pages = kmalloc_array(nr_pages, sizeof(struct page *), GFP_KERNEL);
    if (!pages)
        return -ENOMEM;

    for (i = 0; i < nr_pages; i++) {
        unsigned long va = start_va + i * PAGE_SIZE;
        phys_addr_t pa = __pa_symbol(va);
        if (!pa) {
            kfree(pages);
            pr_err("renameat_hook: __pa_symbol failed for va 0x%lx\n", va);
            return -ENOMEM;
        }
        pages[i] = pfn_to_page(pa >> PAGE_SHIFT);
        if (!pages[i]) {
            kfree(pages);
            pr_err("renameat_hook: pfn_to_page failed for va 0x%lx\n", va);
            return -ENOMEM;
        }
    }

    mapped_table = vmap(pages, nr_pages, VM_MAP, PAGE_KERNEL);
    kfree(pages);

    if (!mapped_table)
        return -ENOMEM;

    return 0;
}

static void cleanup_table_mapping(void)
{
    if (mapped_table) {
        vunmap(mapped_table);
        mapped_table = NULL;
    }
    mapped_base_va = 0;
}

static int safe_write_ulong(int index, unsigned long value)
{
    unsigned long abs_addr;
    unsigned long offset;
    unsigned long flags;

    spin_lock_irqsave(&hook_lock, flags);
    if (!mapped_table) {
        spin_unlock_irqrestore(&hook_lock, flags);
        return -EINVAL;
    }

    abs_addr = (unsigned long)(sys_call_table_addr + index);
    offset = abs_addr - mapped_base_va;
    WRITE_ONCE(mapped_table[offset / sizeof(unsigned long)], value);
    smp_wmb();
    spin_unlock_irqrestore(&hook_lock, flags);
    return 0;
}

static unsigned long manual_lookup_symbol(const char *symbol_name)
{
    struct file *fp;
    char *buf, *line, *p;
    unsigned long addr = 0;
    loff_t pos = 0;
    ssize_t bytes_read;

    buf = kmalloc(4096, GFP_KERNEL);
    if (!buf)
        return 0;

    fp = filp_open("/proc/kallsyms", O_RDONLY, 0);
    if (IS_ERR(fp)) {
        kfree(buf);
        return 0;
    }

    while ((bytes_read = kernel_read(fp, buf, 4095, &pos)) > 0) {
        buf[bytes_read] = '\0';
        p = buf;
        while ((line = strsep(&p, "\n")) != NULL) {
            char *cur_addr_str = strsep(&line, " ");
            (void)strsep(&line, " ");
            char *cur_name = line;

            if (cur_name && strcmp(cur_name, symbol_name) == 0) {
                addr = simple_strtoull(cur_addr_str, NULL, 16);
                if (addr != 0)
                    goto found;
            }
        }
    }

found:
    filp_close(fp, NULL);
    kfree(buf);
    return addr;
}

static asmlinkage long hooked_renameat(const struct pt_regs *regs)
{
    unsigned long args[4];
    char old_path[HOOK_PATH_LOG_MAX];
    char new_path[HOOK_PATH_LOG_MAX];

    memset(args, 0, sizeof(args));
    get_syscall_args(regs, args, 4);
    copy_user_path(old_path, sizeof(old_path), (const char __user *)args[1]);
    copy_user_path(new_path, sizeof(new_path), (const char __user *)args[3]);

    pr_info_ratelimited("renameat_hook: renameat olddfd=%ld old=\"%s\" newdfd=%ld new=\"%s\"\n",
                        (long)args[0], old_path, (long)args[2], new_path);
    return orig_renameat(regs);
}

static asmlinkage long hooked_renameat2(const struct pt_regs *regs)
{
    unsigned long args[5];
    char old_path[HOOK_PATH_LOG_MAX];
    char new_path[HOOK_PATH_LOG_MAX];

    memset(args, 0, sizeof(args));
    get_syscall_args(regs, args, 5);
    copy_user_path(old_path, sizeof(old_path), (const char __user *)args[1]);
    copy_user_path(new_path, sizeof(new_path), (const char __user *)args[3]);

    pr_info_ratelimited("renameat_hook: renameat2 olddfd=%ld old=\"%s\" newdfd=%ld new=\"%s\" flags=0x%lx\n",
                        (long)args[0], old_path, (long)args[2], new_path, args[4]);
    return orig_renameat2(regs);
}

static int __init hook_init(void)
{
    int ret;

    sys_call_table_addr = (unsigned long *)manual_lookup_symbol("sys_call_table");
    if (!sys_call_table_addr) {
        pr_err("renameat_hook: sys_call_table not found\n");
        return -EFAULT;
    }

    ret = init_table_mapping();
    if (ret) {
        pr_err("renameat_hook: init_table_mapping failed: %d\n", ret);
        return ret;
    }

    orig_renameat = (sys_renameat_regs_t)sys_call_table_addr[__NR_renameat];

    if (safe_write_ulong(__NR_renameat, (unsigned long)hooked_renameat) != 0) {
        pr_err("renameat_hook: Patch renameat failed\n");
        cleanup_table_mapping();
        return -EPERM;
    }

    orig_renameat2 = (sys_renameat2_regs_t)sys_call_table_addr[__NR_renameat2];

    if (safe_write_ulong(__NR_renameat2, (unsigned long)hooked_renameat2) != 0) {
        pr_err("renameat_hook: Patch renameat2 failed\n");
        safe_write_ulong(__NR_renameat, (unsigned long)orig_renameat);
        cleanup_table_mapping();
        return -EPERM;
    }

    pr_info("renameat_hook: Applied, table=%px renameat[%d]=%px renameat2[%d]=%px\n",
            sys_call_table_addr, __NR_renameat, hooked_renameat, __NR_renameat2, hooked_renameat2);
    return 0;
}

static void __exit hook_exit(void)
{
    if (sys_call_table_addr) {
        if (orig_renameat)
            safe_write_ulong(__NR_renameat, (unsigned long)orig_renameat);
        if (orig_renameat2)
            safe_write_ulong(__NR_renameat2, (unsigned long)orig_renameat2);
        cleanup_table_mapping();
        pr_info("renameat_hook: Removed successfully\n");
    }
}

module_init(hook_init);
module_exit(hook_exit);
