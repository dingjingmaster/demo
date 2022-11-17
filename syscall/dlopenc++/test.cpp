/*************************************************************************
> FileName: test.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 16 Nov 2022 11:24:17 AM CST
 ************************************************************************/
#include "test.h"

namespace aa
{
    struct A {int a; float b;};
static struct A aa = {1, 2};
Base::Base ()
{
    printf ("base 构造\n");
}

void Base::run ()
{
    printf ("base run\n");
}


Test::Test ()
{
    printf ("test 构造\n");
}

Test::~Test ()
{
    printf ("test 析构\n");
}

int Test::getA ()
{
    return 1;
}


void Test::run ()
{
    printf ("test run\n");
}
}
