/*************************************************************************
> FileName: crc64.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 20 Aug 2024 09:50:11 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t crc64(uint64_t crc, const uint8_t* data, size_t size);


int main (int argc, char* argv[])
{
    printf ("%d\n\n", sizeof(int));
    char* str = "ABCDEFGH";
    uint64_t crc = crc64(0, str, strlen(str) * sizeof(uint8_t));
    printf ("%lu(%0x)\n", crc, crc);

    str = "ABCDEFGHIJKLMNOP";
    crc = crc64(0, str, strlen(str) * sizeof(uint8_t));
    printf ("%lu(%0x)\n", crc, crc);

    return 0;
}


static uint64_t crc64(uint64_t crc, const uint8_t* data, size_t size)
{
    static uint64_t polynomial = 0x9a6c9329ac4bc9b5ULL;
    static uint64_t xorout = 0xffffffffffffffffULL;
    static uint64_t table[256];

    crc ^= xorout;

    if (data == NULL) {
    /* generate the table of CRC remainders for all possible bytes */
        uint64_t c;
        uint32_t i, j;
        for (i = 0;  i < 256;  i++) {
            c = i;
            for (j = 0;  j < 8;  j++) {
                if (c & 1)
                    c = polynomial ^ (c >> 1);
                else
                    c = (c >> 1);
            }
            table[i] = c;
        }
    } else
        while (size) {
            crc = table[(crc ^ *data) & 0xff] ^ (crc >> 8);
            size--;
            data++;
        }

    crc ^= xorout;

    return crc;
}
