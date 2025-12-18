/*************************************************************************
> FileName: ptr.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年12月18日 星期四 15时21分33秒
 ************************************************************************/
#include <stdio.h>

typedef long (*Fn) (void* );

int main (void)
{
    void* p1 = NULL;

    printf ("p1      : %p\n", p1);
    printf ("p1 + 1  : %p\n", p1 + 1);
    printf ("p1 + fn : %p\n", (Fn)(p1 + 1));
    printf ("p1 + fn2: %p\n", ((Fn)p1) + 1);

    return 0;
}
