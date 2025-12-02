/*************************************************************************
> FileName: str-hash.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年12月02日 星期二 17时53分47秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int djb2_hash(const char *str) 
{
    // 初始种子值，通常使用 5381
    unsigned long hash = 5381;
    int c;

    if (str == NULL) {
        return 0; // 处理空指针情况
    }

    // 遍历字符串直到遇到空终止符 '\0'
    while ((c = *str++)) {
        // 核心运算：hash * 33 + c
        // 利用位移操作优化乘法：(hash << 5) 等于 hash * 32
        hash = ((hash << 5) + hash) + c;

        // 等效的简单乘法形式：
        // hash = (hash * 33) + c;
    }

    return hash;
}

int main (void)
{
    const char* str1 = "Hello GLib Hash2";
    const char* str2 = "Hello GLib Hash1";

    printf ("hash: %s -> %lu\n", str1, djb2_hash(str1));
    printf ("hash: %s -> %lu\n", str1, djb2_hash(str2));


    return 0;
}
