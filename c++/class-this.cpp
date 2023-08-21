/*************************************************************************
> FileName: class-this.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 21 Aug 2023 03:44:26 PM CST
 ************************************************************************/
#include <stdio.h>
#include <iostream>

class MyClass
{
public:
    void print ()
    {
        // 在 x86 架构中，this指针通常存储在 ECX(32位) 或 RCX(64位)寄存器中
        void* thisPtr = NULL;
        int i = 0;

        asm volatile(
                "movq %0, %%RCX;"
                : "=r" (thisPtr)
        );

        printf ("this pointer: 0x%X -- 0x%X\n", this, thisPtr);
    }
};

int main (int argc, char* argv[])
{
    MyClass cls;

    cls.print();

    return 0;
}
