/*************************************************************************
> FileName: demo1.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 03 Aug 2023 04:15:23 PM CST
 ************************************************************************/
#include <stdio.h>
#include <functional>

class MyClass
{
public:
    int         x;      // 0
    double      y;      //
    char        z;
protected:
    int         m;
private:
    int         n;
public:
    int         i;
    void foo() {printf ("foo\n");};
};

int main() 
{
    MyClass b;      // 假设首地址未知
    MyClass c;
    
    std::function<void()> fooP = std::bind(&MyClass::foo, &c);

    printf ("size: %d\n", sizeof (MyClass));
    printf (" start : %p\n", &c);           // 70
    printf (" x     : %p\n", &(c.x));       // 70
    printf (" y     : %p\n", &(c.y));       // 78
    printf (" z     : %p\n", &(c.z));       // 80
    printf (" i     : %p\n", &(c.i));       // 8c
    printf (" foo() : %p\n", fooP);         // b0
    printf ("\n");

    char* cObjP = reinterpret_cast<char*>(&c);
    char* cFooP = reinterpret_cast<char*>(&fooP);
    ptrdiff_t offset = cFooP - cObjP;
    printf (" offset: %p(%d)\n", offset, offset);


    // 假设通过 Hook 获得的函数地址
    std::function<void()> fooPb = std::bind(&MyClass::foo, &b);

    // 获取 bThis
    char* bFooPbTT = reinterpret_cast<char*>(&fooPb);
    char* pTmp = bFooPbTT - offset;
    MyClass* bThis = reinterpret_cast<MyClass*>(pTmp);
    printf ("bThis: %p -- %p\n", bThis, &b);

    b.foo();
    bThis->foo();

    return 0;
}
