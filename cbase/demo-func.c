/*************************************************************************
> FileName: demo-func.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 10:32:40 AM CST
 ************************************************************************/
#include <stdio.h>

int i;

void print_val (void)
{
    printf ("%d\n", i);
}

int i = 10;

int main (int argc, char* argv[])
{
    printf ("%d\n", i);
    print_val();
}

// 勿容置疑两个都是 10
