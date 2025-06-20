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

    printf ("%s\n", bt);

    return 0;
}
