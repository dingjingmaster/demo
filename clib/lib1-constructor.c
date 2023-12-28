/*************************************************************************
> FileName: demo1-library-constructor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 28 Dec 2023 02:07:05 PM CST
 ************************************************************************/
#include <stdio.h>

void __attribute__((constructor)) init_func (void)
{
    printf ("Dynamic library loaded!\n");
}

void __attribute__((destructor)) dinit_func (void)
{
    printf ("Dynamic library unloaded!\n");
}
