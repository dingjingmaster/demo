#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/limits.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DingJing");
MODULE_DESCRIPTION("Kretprobe syscall renameat/renameat2 hook with Syscall Wrapper support");

/*
 * Syscall Wrapper (since 4.17 x86, and modern ARM64):
 * The function hooked is long sys_xyz(const struct pt_regs *regs);
 * So the actual registers containing the syscall arguments are pointed to by the 1st argument.
 */
#if defined(CONFIG_X86_64)
    #define GET_REAL_REGS(regs) ((struct pt_regs *)(regs->di))
    #define SYSCALL_PREFIX "__x64_sys_"
#elif defined(CONFIG_ARM64)
    #define GET_REAL_REGS(regs) ((struct pt_regs *)(regs->regs[0]))
    #define SYSCALL_PREFIX "__arm64_sys_"
#else
    #define GET_REAL_REGS(regs) (regs)
    #define SYSCALL_PREFIX "sys_"
#endif

/*
 * Data structure to pass information from entry_handler to ret_handler
 */
struct rename_data {
    char oldname[NAME_MAX];
    char newname[NAME_MAX];
    int is_renameat2;
    unsigned int flags;
};

static void safe_get_filename(const char __user *user_ptr, char *kernel_buf)
{
    if (!user_ptr || strncpy_from_user(kernel_buf, user_ptr, NAME_MAX) < 0)
        strcpy(kernel_buf, "<fault>");
}

/*
 * Entry handler: called BEFORE the syscall
 */
static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct rename_data *data;
    struct pt_regs *real_regs;

    if (!current->mm) return 0;

    /* 
     * IMPORTANT: Handle Syscall Wrapper indirection.
     * 'regs' contains the CPU state when the wrapper function was called.
     * 'real_regs' is the pointer to the actual syscall arguments.
     */
    real_regs = GET_REAL_REGS(regs);
    if (!real_regs) return 0;

    data = (struct rename_data *)ri->data;
    
    /* 
     * In Syscall Wrapper, real_regs contains:
     * arg0: olddfd, arg1: oldname, arg2: newdfd, arg3: newname, arg4: flags
     */
#if defined(CONFIG_X86_64)
    safe_get_filename((char __user *)real_regs->si, data->oldname);
    safe_get_filename((char __user *)real_regs->r10, data->newname);
#elif defined(CONFIG_ARM64)
    safe_get_filename((char __user *)real_regs->regs[1], data->oldname);
    safe_get_filename((char __user *)real_regs->regs[3], data->newname);
#else
    /* Fallback if no wrapper */
    safe_get_filename((char __user *)regs->regs[1], data->oldname);
    safe_get_filename((char __user *)regs->regs[3], data->newname);
#endif

    data->is_renameat2 = (strstr(ri->rp->kp.symbol_name, "renameat2") != NULL);
    if (data->is_renameat2) {
#if defined(CONFIG_X86_64)
        data->flags = (unsigned int)real_regs->r8;
#elif defined(CONFIG_ARM64)
        data->flags = (unsigned int)real_regs->regs[4];
#endif
        pr_info("kretprobe: [BEFORE] renameat2: %s -> %s (flags: 0x%x)\n", data->oldname, data->newname, data->flags);
    } else {
        pr_info("kretprobe: [BEFORE] renameat: %s -> %s\n", data->oldname, data->newname);
    }

    return 0;
}

static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct rename_data *data = (struct rename_data *)ri->data;
    long retval = regs_return_value(regs);

    pr_info("kretprobe: [AFTER] %s finished with return value: %ld\n", 
            data->is_renameat2 ? "renameat2" : "renameat", retval);

    return 0;
}

static struct kretprobe rp_renameat = {
    .handler = ret_handler,
    .entry_handler = entry_handler,
    .data_size = sizeof(struct rename_data),
    .maxactive = 64,
};

static struct kretprobe rp_renameat2 = {
    .handler = ret_handler,
    .entry_handler = entry_handler,
    .data_size = sizeof(struct rename_data),
    .maxactive = 64,
};

static int __init hook_init(void)
{
    int ret;

    rp_renameat.kp.symbol_name = SYSCALL_PREFIX "renameat";
    rp_renameat2.kp.symbol_name = SYSCALL_PREFIX "renameat2";

    ret = register_kretprobe(&rp_renameat);
    if (ret < 0) {
        rp_renameat.kp.symbol_name = "sys_renameat";
        ret = register_kretprobe(&rp_renameat);
    }
    if (ret >= 0) pr_info("kretprobe: Hooked %s\n", rp_renameat.kp.symbol_name);

    ret = register_kretprobe(&rp_renameat2);
    if (ret < 0) {
        rp_renameat2.kp.symbol_name = "sys_renameat2";
        ret = register_kretprobe(&rp_renameat2);
    }
    if (ret >= 0) pr_info("kretprobe: Hooked %s\n", rp_renameat2.kp.symbol_name);

    return 0;
}

static void __exit hook_exit(void)
{
    unregister_kretprobe(&rp_renameat);
    unregister_kretprobe(&rp_renameat2);
    pr_info("kretprobe: Module unloaded\n");
}

module_init(hook_init);
module_exit(hook_exit);
