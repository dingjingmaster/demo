/*************************************************************************
> FileName: demo6.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月19日 星期四 15时12分29秒
 ************************************************************************/
/**
 * 正常实现闭包的逻辑
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

typedef int (*func) (void*, void*);

static void compare (void* a, void* b, func callback)
{
    int r = callback (a, b);
    if (r == -1) {
        printf ("a < b\n");
    } else if (r == 0) {
        printf ("a = b\n");
    } else {
        printf ("a > b\n");
    }
}

static int float_compare (void*a, void*b)
{
    float* f1 = (float*) a;
    float* f2 = (float*) b;

    if (*f1 > *f2) {
        return 1;
    } else if (fabs (*f1 - *f2) <= 10E-6) {
        return 0;
    } else {
        return -1;
    }
}

static int str_compare (void* a, void* b)
{
    size_t len1 = strlen ((char*)a);
    size_t len2 = strlen ((char*)b);

    if (len1 > len2) {
        return 1;
    } else if (len1 == len2) {
        return 0;
    } else {
        return -1;
    }
}

int main (void)
{
    float a = 123.567;
    float b = 222.222;

    func f = float_compare;
    compare (&a, &b, f);

    char* s1 = "hello word!";
    char* s2 = "helo";

    f = str_compare;
    compare (s1, s2, f);

    return 0;
}
