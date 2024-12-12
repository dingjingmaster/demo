#include <linux/fs.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <generated/autoconf.h>

#ifdef CONFIG_KALLSYMS
#ifdef CONFIG_KALLSYMS_ALL
/**
 * @brief 存在返回 0
 */
static int efs_file_exist(const char* path)
{
    int fileExists = 1;
    struct file* file = NULL;

    file = filp_open(path, O_RDONLY, 0);
    if (!IS_ERR(file)) {
        fileExists = 0;
        filp_close(file, NULL);
    }

    return fileExists;
}

/**
 * @brief 写入字符串, 成功返回0
 */
static int efs_file_write_str(const char* path, const char* str, int mode)
{
    struct file* file = NULL;

    file = filp_open(path, O_WRONLY | O_TRUNC, mode);
    if (!IS_ERR(file)) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
        mm_segment_t oldFs = get_fs();
        set_fs(KERNEL_DS);
#endif
        kernel_write(file, str, strlen(str), 0);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
        set_fs(oldFs);
#endif
        filp_close(file, NULL);
        return 0;
    }

    return 1;
}

static unsigned long efs_get_symbol_by_proc_kallsyms(const char* name)
{
#define BUFFER_SIZE 256
#define EFS_KALLSYMS_PATH   "/proc/kallsyms"

    char* buffer = NULL;
    loff_t filePos = 0;
    struct file* file = NULL;

    do {
        buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
        if (!buffer) {
            printk(KERN_ERR "kmalloc failed!\n");
            break;
        }

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
        mm_segment_t oldFs = get_fs();
        set_fs(KERNEL_DS);
#endif
        file = filp_open (EFS_KALLSYMS_PATH, O_RDONLY, 0);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
        set_fs(oldFs);
#endif

        if (IS_ERR(file)) {
            printk(KERN_ERR "open file: %s, error\n", EFS_KALLSYMS_PATH);
            break;
        }

        while (kernel_read(file, buffer, BUFFER_SIZE - 1, &file->f_pos) > 0) {
            buffer[BUFFER_SIZE - 1] = '\0';
            char* curPos = strstr(buffer, "\n");
            curPos = '\0';
            filePos = strlen(buffer);
            file->f_pos = filePos + 1;

            char type;
            unsigned long addr = 0;
            char name[BUFFER_SIZE] = {0};

            if (3 == sscanf(buffer, "%lx %c %s", &addr, &type, name)) {
                printk(KERN_ERR "Address: %lx, type: %c, name: %s\n", addr, type, name);
            }
        }
    } while (0);

    if (file) {
        filp_close(file, NULL);
    }

    if (buffer) {
        kfree(buffer);
    }

    return 0;
}

static unsigned long efs_get_symbol_address(const char* name)
{
#define EFS_KALLSYMS_CTL    "/proc/sys/kernel/kptr_restrict"
#define EFS_KALLSYMS_PATH   "/proc/kallsyms"
    if (0 == efs_file_exist(EFS_KALLSYMS_CTL)) {
        efs_file_write_str(EFS_KALLSYMS_CTL, "1\n", 0444);
        if (0 == efs_file_exist(EFS_KALLSYMS_PATH)) {
            efs_get_symbol_by_proc_kallsyms(name);
        }
        efs_file_write_str(EFS_KALLSYMS_CTL, "2\n", 0444);
    }

    return 0;
}
#define EFS_HAS_KALLSYMS_LOOKUP_NAME
#endif
#endif

static int __init symbol_traverse_init(void)
{
    printk(KERN_INFO "Start symbol\n");

#ifdef EFS_HAS_KALLSYMS_LOOKUP_NAME
    efs_get_symbol_address(NULL);
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
