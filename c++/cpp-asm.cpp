/*************************************************************************
> FileName: cpp-asm.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 21 Aug 2023 04:18:10 PM CST
 ************************************************************************/
#include <stdio.h>

int main (void)
{
    int foo = 10, bar = 15;

    asm volatile (
            "addl %%ebx,%%eax"
            : "=a"(foo)
            : "a"(foo), "b"(bar)
    );

    printf ("foo = %d\n", foo);
}
