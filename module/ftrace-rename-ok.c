#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/ftrace.h>
#include <linux/ptrace.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/limits.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DingJing");
MODULE_DESCRIPTION("Ftrace syscall rename hook (Harden v3)");

/*
 * Use kprobe to find kallsyms_lookup_name since it's not exported since 5.7
 */
static unsigned long (*kallsyms_lookup_name_ptr)(const char *name);

static int find_kallsyms_lookup_name(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0)
    struct kprobe kp = {
        .symbol_name = "kallsyms_lookup_name",
    };
    int ret = register_kprobe(&kp);
    if (ret < 0)
        return ret;

    kallsyms_lookup_name_ptr = (unsigned long (*)(const char *))kp.addr;
    unregister_kprobe(&kp);
    return 0;
#else
    kallsyms_lookup_name_ptr = (unsigned long (*)(const char *))kallsyms_lookup_name;
    return 0;
#endif
}

/*
 * Hook structure
 */
struct ftrace_hook {
    const char *name;
    void *wrapper;
    void *original;
    unsigned long address;
    struct ftrace_ops ops;
};

/*
 * Use a more robust recursion check: if parent_ip is not inside our module,
 * we redirect. If it is, we let the original function run.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops, struct ftrace_regs *fregs)
{
    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);
    struct pt_regs *regs = ftrace_get_regs(fregs);

    if (unlikely(!regs)) return;

    if (!within_module(parent_ip, THIS_MODULE)) {
        regs->ip = (unsigned long)hook->wrapper;
    }
}
#else
static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops, struct pt_regs *regs)
{
    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

    if (unlikely(!regs)) return;

    if (!within_module(parent_ip, THIS_MODULE)) {
        regs->ip = (unsigned long)hook->wrapper;
    }
}
#endif

static int fh_install_hook(struct ftrace_hook *hook)
{
    hook->address = kallsyms_lookup_name_ptr(hook->name);
    if (!hook->address) {
        pr_err("ftrace_hook: [ERROR] Failed to resolve symbol %s\n", hook->name);
        return -ENOENT;
    }

    /* 
     * In modern kernels, calling the address of a symbol that is already 
     * being traced will cause a recursion. Our thunk handles this, but 
     * we must set the original pointer first.
     */
    *((unsigned long *)hook->original) = hook->address;

    hook->ops.func = fh_ftrace_thunk;
    hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY;

#ifdef FTRACE_OPS_FL_RECURSION
    hook->ops.flags |= FTRACE_OPS_FL_RECURSION;
#else
    hook->ops.flags |= FTRACE_OPS_FL_RECURSION_SAFE;
#endif

    int ret = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
    if (ret) {
        pr_err("ftrace_hook: [ERROR] ftrace_set_filter_ip() failed: %d\n", ret);
        return ret;
    }

    ret = register_ftrace_function(&hook->ops);
    if (ret) {
        pr_err("ftrace_hook: [ERROR] register_ftrace_function() failed: %d\n", ret);
        ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
        return ret;
    }

    return 0;
}

static void fh_remove_hook(struct ftrace_hook *hook)
{
    unregister_ftrace_function(&hook->ops);
    ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
}

/*
 * Syscall wrapper.
 * On 5.4.0 (Ubuntu 18.04/20.04), syscalls use the x86-64 wrapper:
 * asmlinkage long __x64_sys_rename(const struct pt_regs *regs)
 */
typedef asmlinkage long (*sys_rename_t)(const struct pt_regs *);
static sys_rename_t original_sys_rename;

/*
 * Safe user string copier
 */
static void safe_get_filename(const char __user *user_ptr, char *kernel_buf)
{
    if (strncpy_from_user(kernel_buf, user_ptr, NAME_MAX) < 0) {
        strcpy(kernel_buf, "<fault>");
    }
}

static asmlinkage long notrace fh_sys_rename(const struct pt_regs *regs)
{
    long ret;
    char *k_oldname, *k_newname;

    /* 
     * Check if original_sys_rename is NULL (paranoia, but needed for 0x0 RIP Oops)
     */
    if (unlikely(!original_sys_rename)) {
        return -EFAULT;
    }

    k_oldname = kmalloc(NAME_MAX, GFP_ATOMIC);
    k_newname = kmalloc(NAME_MAX, GFP_ATOMIC);

    if (k_oldname && k_newname) {
        /* On x86-64, parameters are in di, si inside pt_regs */
        safe_get_filename((char __user *)regs->di, k_oldname);
        safe_get_filename((char __user *)regs->si, k_newname);

        pr_info("ftrace_hook: [BEFORE] rename: %s -> %s\n", k_oldname, k_newname);
    }

    /* CALL ORIGINAL */
    ret = original_sys_rename(regs);

    pr_info("ftrace_hook: [AFTER] rename finished: %ld\n", ret);

    kfree(k_oldname);
    kfree(k_newname);

    return ret;
}

static struct ftrace_hook rename_hook = {
    .name = "__x64_sys_rename",
    .wrapper = fh_sys_rename,
    .original = &original_sys_rename,
};

static int __init rename_hook_init(void)
{
    int ret;

    ret = find_kallsyms_lookup_name();
    if (ret) {
        pr_err("ftrace_hook: [ERROR] Failed to find kallsyms_lookup_name: %d\n", ret);
        return ret;
    }

    /* Check if symbol exists before attempting hook */
    if (!kallsyms_lookup_name_ptr(rename_hook.name)) {
        pr_info("ftrace_hook: [INFO] __x64_sys_rename not found, trying sys_rename\n");
        rename_hook.name = "sys_rename";
    }

    ret = fh_install_hook(&rename_hook);
    if (ret) {
        pr_err("ftrace_hook: [ERROR] Failed to install hook: %d\n", ret);
        return ret;
    }

    pr_info("ftrace_hook: [SUCCESS] Hooked %s\n", rename_hook.name);
    return 0;
}

static void __exit rename_hook_exit(void)
{
    fh_remove_hook(&rename_hook);
    pr_info("ftrace_hook: [INFO] Module unloaded\n");
}

module_init(rename_hook_init);
module_exit(rename_hook_exit);
