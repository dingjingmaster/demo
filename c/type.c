/*************************************************************************
> FileName: type.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 17 Mar 2023 10:21:18 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>

int main (int argc, char* argv[])
{
    printf ("uint8_t: %u\n", sizeof(uint8_t));
    printf ("uint16_t: %u\n", sizeof(uint16_t));
    printf ("uint32_t: %u\n", sizeof(uint32_t));
    printf ("uint64_t: %u\n", sizeof(uint64_t));
    printf ("unsigned char: %u\n", sizeof(unsigned char));
    printf ("unsigned short: %u\n", sizeof(unsigned short));
    printf ("unsigned int: %u\n", sizeof(unsigned int));
    printf ("unsigned long: %u\n", sizeof(unsigned long));
    printf ("unsigned sort: %u\n", sizeof(unsigned short));


    return 0;
}
