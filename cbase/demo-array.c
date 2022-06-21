/*************************************************************************
> FileName: demo-array.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 11:14:54 AM CST
 ************************************************************************/
#include <stdio.h>

// 打印数组第二个值
int main (void)
{
    int a[5] = {1, 2, 3, 4, 5};

    int *p = a;

    printf ("%d\n", *(p + 2));          // ok
    printf ("%d\n", *(&a + 2));         // error 指针步长
    printf ("%d\n", 2[p]);              // ok
    printf ("%d\n", 2[a]);              // ok


    //printf ("===>\n");
    //printf ("((&a + 2) - &a):%d\n", (&a + 2) - p); // &a + 2 无法访问值

    return 0;
}
