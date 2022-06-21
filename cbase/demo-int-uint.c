/*************************************************************************
> FileName: demo-int-uint.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 10:21:21 AM CST
 ************************************************************************/
#include <stdio.h>

int main (int argc, char* argv[])
{
    int a = -3;
    unsigned int b = 3;

    if (a < b) {
        printf ("a:%d, b:%d --> a<b\n", a, b);
    } else {
        printf ("a:%d, b:%d --> a>b\n", a, b);
    }

    // 解决是 a>b， 都以 unsigned int 比较

    return 0;
}
