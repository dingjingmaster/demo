/*************************************************************************
> FileName: bytes-to-utf8.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月20日 星期五 10时10分07秒
 ************************************************************************/
#include <stdio.h>


int main (void)
{
    const char* bt = "\xe5\x8a\xa0\xe5\xaf\x86\xe6\x96\x87\xe4\xbb\xb6\xef\xbc\x8c\xe4\xb8\x8d\xe8\xa6\x81\xe7\xbc\x96\xe8\xbe\x91";

    const char* magic = "MRD ";
    const char* magic2 = "SEC";

    printf ("tip    : %s\n", bt);
    printf ("magic  : 0x");
    for (int i = 0; magic[i]; ++i) {
        printf ("%X", magic[i]);
    }
    printf("\n");
    printf ("magic1 : 0x");
    printf ("%c", 0x53);
    printf ("%c", 0x45);
    printf ("%c", 0x43);
    printf ("%c", 0x18);
    printf("\n");

    printf ("magic2 : 0x");
    for (int i = 0; magic2[i]; ++i) {
        printf ("%X", magic2[i]);
    }
    printf("\n");

    return 0;
}
