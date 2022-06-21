/*************************************************************************
> FileName: demo-array-pointer.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 10:26:58 AM CST
 ************************************************************************/
#include <stdio.h>

int main (int argc, char* argv[])
{
    int a[5] = {1, 2, 3, 4, 5};
    int *p = NULL;

    p = a;
    printf ("a:%p -- p:%p\n", a, p);

    p = &a;
    printf ("a:%p -- p:%p\n", a, p);

    p = &a[0];
    printf ("a:%p -- p<==>[&a[0]]:%p\n", a, p);

    p = &a[4];
    printf ("a:%p -- p<==>[&a[4]]:%p\n", a, p);

    return 0;
}
