/*************************************************************************
> FileName: sizeof.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 23 Aug 2023 11:05:04 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>

int main (int argc, char* argv[])
{
    printf ("void*                  : %d\n", sizeof(void*));
    printf ("unsigned long          : %d\n", sizeof(unsigned long));
    printf ("unsigned long long     : %d\n", sizeof(unsigned long long));
    printf ("uint32_t               : %d\n", sizeof(uint32_t));

    return 0;
}
