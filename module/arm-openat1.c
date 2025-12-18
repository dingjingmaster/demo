#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>

MODULE_LICENSE("GPL");

typedef long (*sys_openat_t)(int dfd, const char __user *filename,
                             int flags, umode_t mode);

static sys_openat_t original_openat = NULL;
static unsigned long *sys_call_table = NULL;

/* ARM64 写保护禁用/启用 */
static inline void write_cr0_forced(unsigned long val)
{
    /* ARM64 没有 CR0，需要修改页表属性 */
}

static void set_page_rw(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    
    if (pte) {
        set_pte_at(&init_mm, addr, pte, 
                   pte_mkwrite(pte_mkdirty(*pte)));
        flush_tlb_all();
    }
}

static void set_page_ro(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    
    if (pte) {
        set_pte_at(&init_mm, addr, pte, pte_wrprotect(*pte));
        flush_tlb_all();
    }
}

/* 自定义 openat 处理函数 */
static long hooked_openat(int dfd, const char __user *filename,
                          int flags, umode_t mode)
{
    char buf[256];
    long ret;
    
    if (strncpy_from_user(buf, filename, sizeof(buf) - 1) > 0) {
        buf[sizeof(buf) - 1] = '\0';
        pr_info("hooked openat: pid=%d, file=%s\n", 
                current->pid, buf);
    }
    
    /* 调用原始函数 */
    ret = original_openat(dfd, filename, flags, mode);
    
    return ret;
}

static unsigned long *find_sys_call_table(void)
{
    return (unsigned long *)kallsyms_lookup_name("sys_call_table");
}

static int __init hook_init(void)
{
    sys_call_table = find_sys_call_table();
    if (!sys_call_table) {
        pr_err("Could not find sys_call_table\n");
        return -EFAULT;
    }
    
    pr_info("sys_call_table found at %px\n", sys_call_table);
    
    /* 保存原始函数指针 */
    original_openat = (sys_openat_t)sys_call_table[__NR_openat];
    
    /* 修改页表使其可写 */
    set_page_rw((unsigned long)sys_call_table);
    
    /* 替换系统调用 */
    sys_call_table[__NR_openat] = (unsigned long)hooked_openat;
    
    /* 恢复只读 */
    set_page_ro((unsigned long)sys_call_table);
    
    pr_info("openat hooked successfully\n");
    return 0;
}

static void __exit hook_exit(void)
{
    if (sys_call_table && original_openat) {
        set_page_rw((unsigned long)sys_call_table);
        sys_call_table[__NR_openat] = (unsigned long)original_openat;
        set_page_ro((unsigned long)sys_call_table);
        pr_info("openat unhooked\n");
    }
}

module_init(hook_init);
module_exit(hook_exit);
