#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/kallsyms.h>

static unsigned long kaddr_lookup_name(void)
{
    int i = 0;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,19,0))
    int s = 0x200000;
#else
    int s = 0x200000;
#endif

    unsigned long kaddr;
    char *fname_lookup = NULL;

    printk(KERN_INFO "kaddr_lookup_name 1\n");;
    fname_lookup = kzalloc(NAME_MAX + 1, GFP_KERNEL);
    if (!fname_lookup) {
        printk(KERN_ERR "kzalloc error\n");
        return 0;
    }

    printk(KERN_INFO "kaddr_lookup_name 2\n");;
    /*
     * We have to add "+0x0" to the end of our function name
     * because that's the format that sprint_symbol() returns
     * to us. If we don't do this, then our search can stop
     * prematurely and give us the wrong function address!
     *
     * Get the kernel base address:
     * sprint_symbol() is less than 0x100000 from the start of the kernel, so
     * we can just AND-out the last 3 bytes from it's address to the the base
     * address.
     * There might be a better symbol-name to use?
     */
    kaddr = (unsigned long) &sprint_symbol;
    kaddr &= 0xffffffffff000000;

    /*
     * All the syscalls (and all interesting kernel functions I've seen so far)
     * are within the first 0x100000 bytes of the base address. However, the kernel
     * functions are all aligned so that the final nibble is 0x0, so we only
     * have to check every 16th address.
     */
    printk(KERN_INFO "kaddr_lookup_name 3\n");;
    for (i = 0x0 ; i < s ; i++) {
        /**
         * @brief sprint_symbol 用于将内核中的符号地址(函数指针或内核变量地址)解析成可读的符号名称,
         *  并以字符串形式返回。
         */
        memset(fname_lookup, 0, NAME_MAX + 1);
        /* int relsize = */ sprint_symbol(fname_lookup, kaddr);

        printk(KERN_INFO "Address: %lx, symbol: %s\n", kaddr, fname_lookup);

        /*
         * Jump 16 addresses to next possible address
         */
        kaddr += 0x10;
    }
    printk(KERN_INFO "kaddr_lookup_name 4\n");;
    /*
     * We didn't find the name, so clean up and return 0
     */
    if (fname_lookup) {
        kfree(fname_lookup);
    }

    printk(KERN_INFO "kaddr_lookup_name 5 OK\n");;

    return 0;
}


static int __init symbol_traverse_init(void)
{
    printk(KERN_INFO "Start symbol\n");

    kaddr_lookup_name();

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
