/*************************************************************************
> FileName: class-size.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Mar 2024 01:45:35 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

// 1
class A
{

};


// 4
class B
{
    int e1;

};

// 4
class C
{
public:
    int e1;

};


// 16
class D
{
public:
    double  e1;
    int     e2;
};

// 16
class E
{
public:
    int     e1;
    double  e2;
};

// 32
class F
{
public:
    int     e1;         // 0 - 3
    double  e2;         // 4 - 11
    char    e3[10];     // 12 - 21
    int     e4;         // 24 - 27
};                      // 28 ? ==> 32大小

// 4
class G
{
public:
    int     e1;
    int f1 (int a, int b) { return b + a; }     // 类中成员函数放到代码段中，并不在类中
};

// 16
class H
{
public:
    int     e1;

    // 虚表指针 8 字节，对齐后则是 15 ==> ?
    virtual int f1 (int a, int b) {return b + a; }
};

// 16
class I
{
public:
    int     e1;

    // 虚表指针 8 字节，对齐后则是 15 ==> ?
    virtual int f1 (int a, int b) {return b + a; }
    virtual int f2 (int a, int b) {return b + a; }
};

/**

   +--------------------------+
   |        父类属性          |
   +--------------------------+
   |        子类属性          |
   +--------------------------+

*/


// 24
class J : public I
{
public:
    double j1;
};

// 24
class K : public I
{
public:
    double j1;
    virtual int f1 (int a, int b) {return b + a; }
};

// 16
class L : public I
{
public:
    int j1;
    virtual int f1 (int a, int b) {return b + a; }
};

// F: 32， 对齐倍数：8
// M：40
class M : public F
{
public:
    int jj1;
};

// F: 32， 对齐倍数：8
// N：48
class N : public F
{
public:
    int jj1;
    virtual int f1 (int a, int b) {return b + a; }
};



int main ()
{
    printf ("void*  :   %lu\n", sizeof(void*)); // 8
    printf ("A      :   %lu\n", sizeof(A));     // 1
    printf ("B      :   %lu\n", sizeof(B));     // 1
    printf ("C      :   %lu\n", sizeof(C));     // 4
    printf ("D      :   %lu\n", sizeof(D));     // 16
    printf ("E      :   %lu\n", sizeof(E));     // 16
    printf ("F      :   %lu\n", sizeof(F));     // 32
    printf ("G      :   %lu\n", sizeof(G));     // 4
    printf ("H      :   %lu\n", sizeof(H));     // 16
    printf ("I      :   %lu\n", sizeof(I));     // 16
    printf ("J      :   %lu\n", sizeof(J));     // 24
    printf ("K      :   %lu\n", sizeof(K));     // 24
    printf ("L      :   %lu\n", sizeof(L));     // 16
    printf ("M      :   %lu\n", sizeof(M));     // 40
    printf ("N      :   %lu\n", sizeof(N));     // 48

    return 0;
}
