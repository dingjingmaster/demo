/*************************************************************************
> FileName: demo-func1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 10:37:23 AM CST
 ************************************************************************/
#include <stdio.h>

int i;

extern void print (void);

int main (void)
{
    // 链接时候报错: 重定义
    printf ("%d\n", sizeof(i));
    print();

    return 0;
}
