/*************************************************************************
> FileName: hello.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月12日 星期二 09时52分34秒
 ************************************************************************/
#ifndef _HELLO_H
#define _HELLO_H

typedef struct
{
    int x;
    int y;
} Point;

void say_hello(const char* name);

int add (int a, int b);

Point* create_point(int x, int y);
void free_point(Point* p);

#endif
