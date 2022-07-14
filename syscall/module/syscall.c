/*************************************************************************
> FileName: syscall.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 12 Jul 2022 01:54:06 PM CST
 ************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>

/**
 * @bref 将系统调用 pref_event_open() 替换为自定义的函数
 *
 */

unsigned long* sys_call_table = NULL;

unsigned int clear_and_return_cr0 (void);

void steback_cr0 (unsigned int val);

static int sys_hackcall (void);

// 定义函数指针，保存原始的系统调用
static int (*orig_syscall_saved) (void);

// 设置 cr0 寄存器第 16 位为0，允许修改系统调用入口表
unsigned int clear_and_return_cr0 (void)
{
    unsigned int cr0 = 0;
    unsigned int ret;

    // 将 cr0 寄存器的值移动到 rax 寄存器中，同时输出到 cr0 变量中
    asm volatile ("movq %%cr0, %%rax" : "=a"(cr0));

    ret = cr0;

    cr0 &= 0xfffeffff; 

    asm volatile ("movq %%rax, %%cr0" :: "a"(cr0));

    return ret;
}

// 还原 cr0 寄存器的值为 val
void setback_cr0 (unsigned int val)
{
    asm volatile ("movq %%rax, %%cr0" :: "a"(val));
}

// 编写自己的系统调用函数
static int sys_hackcall (void)
{
    printk ("Hack syscall is successful!!!\n");
    printk ("拦截成功!!!\n");

    return 0;
}


///////////////////// main
static int __init init_hack_module (void)
{
    int orig_cr0;

    printk ("Hack syscall is starting ...\n");

    // 获取 sys_call_table 虚拟内存地址
    sys_call_table = (unsigned long *) kallsyms_lookup_name ("sys_call_table");

    // 保留原始系统调用
    orig_syscall_saved = (int(*)(void)) (sys_call_table[__NR_perf_event_open]);

    // 设置寄存器值的第16位为0
    orig_cr0 = clear_and_return_cr0 (); 

    // 替换为自定义的函数
    sys_call_table[__NR_perf_event_open] = (unsigned long) & sys_hackcall;

    // 还原 cr0 寄存器的值
    setback_cr0(orig_cr0);

    return 0;
}

static void __exit exit_hack_module (void)
{
    int orig_cr0;

    orig_cr0 = clear_and_return_cr0();

    // 设置为原来的系统调用
    sys_call_table[__NR_perf_event_open] = (unsigned long) orig_syscall_saved;

    setback_cr0 (orig_cr0);

    printk ("Hack syscall is exited ...\n");
}


module_init (init_hack_module);
module_exit (exit_hack_module);

MODULE_LICENSE ("GPL");
