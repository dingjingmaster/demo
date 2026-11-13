/*
 * hello-1.c - The simplest kernel module.
 */
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

static int __init f_init(void)
{
    pr_info("Hello world 1.\n");

#ifdef vfs_read
    pr_info("vfs_read is defined\n");
#else
    pr_info("vfs_read is not defined\n");
#endif

    /* A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}

static void __exit f_cleanup(void)
{
    pr_info("Goodbye world 1.\n");
}

MODULE_LICENSE("GPL");
module_init(f_init);
module_exit(f_cleanup);
