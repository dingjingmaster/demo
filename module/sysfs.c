#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

static struct kobject *example_kobj;
static int value = 0;

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int ret;
    ret = kstrtoint(buf, 10, &value);
    if (ret < 0)
        return ret;
    return count;
}

static struct kobj_attribute value_attribute = 
    __ATTR(value, 0660, value_show, value_store);

static int __init example_init(void)
{
    int retval;

    example_kobj = kobject_create_and_add("example", kernel_kobj);
    if (!example_kobj)
        return -ENOMEM;

    retval = sysfs_create_file(example_kobj, &value_attribute.attr);
    if (retval)
        kobject_put(example_kobj);

    printk(KERN_INFO "sysfs_example: 模块已加载\n");
    return retval;
}

static void __exit example_exit(void)
{
    kobject_put(example_kobj);
    printk(KERN_INFO "sysfs_example: 模块已卸载\n");
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("一个简单的 sysfs 示例模块"); 