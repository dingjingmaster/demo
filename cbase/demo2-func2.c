/*************************************************************************
> FileName: demo-func1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 10:37:23 AM CST
 ************************************************************************/
#include <stdio.h>

void print (void);

// 重定义
//int i = 20;
extern int i;

int main (void)
{
    // 链接时候报错: 重定义
    printf ("%d\n", i);
    print();

    return 0;
}
