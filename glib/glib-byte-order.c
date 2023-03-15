/*************************************************************************
> FileName: glib-byte-order.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 15 Mar 2023 09:23:30 AM CST
 ************************************************************************/
#include <glib.h>


int main (int argc, char* argv[])
{
    printf ("%s\n", (G_BYTE_ORDER == G_LITTLE_ENDIAN ? "小端" : "大端"));

    int num = 0x12345678;
    printf ("int: %d\n", sizeof (num));

    printf ("小端:\n");
    printf ("[0]: 0x%2x\n", *((char*)&num + 0));
    printf ("[1]: 0x%2x\n", *((char*)&num + 1));
    printf ("[2]: 0x%2x\n", *((char*)&num + 2));
    printf ("[3]: 0x%2x\n", *((char*)&num + 3));

    printf ("大端:\n");
    num = GINT_TO_BE(num);
    printf ("[0]: 0x%2x\n", *((char*)&num + 0));
    printf ("[1]: 0x%2x\n", *((char*)&num + 1));
    printf ("[2]: 0x%2x\n", *((char*)&num + 2));
    printf ("[3]: 0x%2x\n", *((char*)&num + 3));

    return 0;
}
