#include <linux/init.h>
#include <linux/module.h>
#include <linux/kallsyms.h>

static int print_symbol_callback(void* data, const char* name, struct module* mod, unsigned long addr)
{
    char buf[1024] = {0};

    sprint_symbol(buf, addr);

    printk(KERN_INFO "Symbol: %s, Address: %lx, Resolved: %s\n", name, addr, buf);

    return 0;
}

static int __init symbol_traverse_init(void)
{
    printk(KERN_INFO "Start symbol\n");
    kallsyms_on_each_symbol((void*)print_symbol_callback, NULL);

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
