/*************************************************************************
> FileName: hello.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月12日 星期二 09时38分19秒
 ************************************************************************/
#include "hello.h"

#include <stdio.h>
#include <stdlib.h>

void say_hello(const char* name)
{
    printf ("Hello, %s!\n", name);
}

int add (int a, int b)
{
    return a + b;
}

Point* create_point(int x, int y)
{
    Point* p = malloc(sizeof(Point));
    if (!p) {
        return NULL;
    }

    p->x = x;
    p->y = y;

    return p;
}

void free_point(Point* p)
{
    if (p) {
        free (p);
    }
}
