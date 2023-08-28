/*************************************************************************
> FileName: init.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 24 Aug 2023 02:42:29 PM CST
 ************************************************************************/
#include <iostream>

class A
{
public:
    //A()=delete;
    A()  { std::cout << "class A" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }
};

class B
{
public:
    B()  { std::cout << "class B" << std::endl; }
    ~B() { std::cout << "~B()" << std::endl; }

public:
    A a;
};

int main (int argc, char* argv[])
{
    B b;

    return 0;
}
