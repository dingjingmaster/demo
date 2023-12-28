/*************************************************************************
> FileName: byte-order.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 28 Dec 2023 05:00:55 PM CST
 ************************************************************************/
#include <stdio.h>

int main ()
{
    int i = 0x01020304;

    printf ("%s\n", ((*(char*)&i == 1) ? "大端" : "小端"));

    return 0;
}
