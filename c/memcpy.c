/*************************************************************************
> FileName: memcpy.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年02月28日 星期一 15时32分41秒
 ************************************************************************/
#include <stdio.h>

void print (int aa[], int len)
{
    for (int i = 0; i < len; ++i) {
        printf ("%d    ", aa[i]);
    }
    printf ("\n");
}

int main (int argc, char* argv[])
{
    int aa[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    print (aa, sizeof (aa) / sizeof (aa[0]));

    memcpy (aa + 3, aa, 5 * sizeof (aa[0]));

    print (aa, sizeof (aa) / sizeof (aa[0]));

    return 0;
}
