/*************************************************************************
> FileName: glib-standard_macros.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 08:49:42 AM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>


int main (int argc, char* argv[])
{
    // (x, low, high) (x > high ? high : (x < low) ? low : x)
    // x > high 返回 high
    // x < low 返回 low
    // x > low && x < high 返回 x
    printf ("%d\n", CLAMP(10, 2, 10));
    printf ("%d\n", CLAMP(10, 2, 11));
    printf ("%d\n", CLAMP(10, 2, 9));
    printf ("%d\n", CLAMP(10, 11, 12));

    // (a, b, epsilon)
    // |a - b| < epsilon 返回ture
    // |a - b| >= epsilon 返回false
    printf ("%s\n", G_APPROX_VALUE(5, 6, 2) ? "true" : "false");
    printf ("%s\n", G_APPROX_VALUE(5, 6, 1) ? "true" : "false");

    // (type)
    struct A
    {
        int a;
        int b;
    };
    struct B
    {
        int a;
        long b;
    };
    struct C
    {
        long b;
        int a;
    };
    struct D
    {
        int a;
    };
    struct E
    {
        int a;
        struct B b;
    };
    printf ("%d -- %d\n", G_ALIGNOF(struct A), sizeof (struct A));
    printf ("%d -- %d\n", G_ALIGNOF(struct B), sizeof (struct B));
    printf ("%d -- %d\n", G_ALIGNOF(struct C), sizeof (struct C));
    printf ("%d -- %d\n", G_ALIGNOF(struct D), sizeof (struct D));
    printf ("%d -- %d\n", G_ALIGNOF(struct E), sizeof (struct E));

    printf ("内存对齐字节:%d\n", G_MEM_ALIGN);

    return 0;
}
