/*************************************************************************
> FileName: test.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 16 Nov 2022 11:22:31 AM CST
 ************************************************************************/
#ifndef _TEST_H
#define _TEST_H
#include <stdio.h>

namespace aa
{
class Base
{
public:
    Base ();
    virtual void run();
};

class Test : public Base
{
public:
    Test();
    ~Test();
    void run() override;
    static int getA();

public:
    int mA = 11;
};
}


#endif
