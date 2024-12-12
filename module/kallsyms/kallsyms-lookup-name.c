#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/kallsyms.h>

#ifdef CONFIG_KALLSYMS
#ifdef CONFIG_KALLSYMS_ALL
extern unsigned long kallsyms_lookup_name(const char* name);
typedef unsigned long (*KallsymsLookupName) (const char* name);
#define EFS_HAS_KALLSYMS_LOOKUP_NAME
#endif
#endif

static int __init symbol_traverse_init(void)
{
    printk(KERN_INFO "Start symbol\n");

#ifdef EFS_HAS_KALLSYMS_LOOKUP_NAME
    KallsymsLookupName kallsymsLookupName = (KallsymsLookupName) kallsyms_lookup_name;
    unsigned long addr = kallsymsLookupName ("sys_call_table");
    printk (KERN_INFO "addr: %lx\n", addr);
#endif

    printk(KERN_ERR "kallsyms_lookup_name undefined!\n");

    return 0;
}

static void __exit symbol_traverse_exit(void)
{
    printk(KERN_INFO "OK\n");
}

module_init(symbol_traverse_init);
module_exit(symbol_traverse_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DingJing");
