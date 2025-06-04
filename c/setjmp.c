/*************************************************************************
> FileName: setjmp.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月04日 星期三 14时41分17秒
 ************************************************************************/
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf gStackEnv;

static void func1 (void);
static void func2 (void);

/**
 * @brief 在main函数中，使用 setjmp 将当前的栈环境保存到
 * gStackEnv 中，然后调用 func1->func2, 在 func2 中，使用
 * longjmp来恢复保存的栈环境 gStackEnv，从而完成长跳转。
 *
 * setjmp和longjmp的实现原理就是对与栈相关的寄存器的保存和恢复。
 */
int main (void)
{
    if (0 == setjmp(gStackEnv)) {
        printf ("Normal flow\n");
        func1();
    }
    else {
        printf ("Longjump flow\n");
    }

    return 0;
}

static void func1 (void)
{
    printf ("Enter func1\n");
    func2();
}

static void func2 (void)
{
    printf ("Enter func2\n");
    longjmp (gStackEnv, 1);
    printf ("Leave func2\n");
}
