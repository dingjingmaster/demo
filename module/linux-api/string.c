/*************************************************************************
> FileName: ./string.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 27 Oct 2023 09:42:56 AM CST
 ************************************************************************/
#include "string.h"

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/string.h>

void dj_demo_string (void)
{
    const char* str1 = "1234qwe4567fff";

    /**
     * 1. 第一个参数是要转换的字符串
     * 2. 第二个参数是转换后剩余字符串的位置，如果转换完就会变成字符串首地址
     */
    unsigned long long l1 = simple_strtoull(str1, NULL, 0); // 0: 自动检测; 2. 二进制; 8; 10; 16
    printk ("l1: %lld", l1); // 结果是 1234

    // unsigned long simple_strtoul(...同上)
    // long simple_strtol(...同上)
    // long long simple_strtoll(...同上)

    printk ("");
}
