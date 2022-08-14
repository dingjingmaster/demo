/*************************************************************************
> FileName: arr.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Sun 14 Aug 2022 01:35:03 PM CST
 ************************************************************************/
#include <stdio.h>

int main ()
{
    char bb[] = "123456";

    char* p = bb;

    while (*p++) {
        if (p[-1] == '\0') {
            break;
        }
        printf ("%c\n", p[-1]);
    }
}
