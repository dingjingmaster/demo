/*************************************************************************
> FileName: version.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 08 Mar 2023 08:56:11 AM CST
 ************************************************************************/
#include <stdio.h>

#include <linux/version.h>

int main (int argc, char* argv[])
{

    printf ("%d\n", KERNEL_VERSION(4, 19, 0));
}
