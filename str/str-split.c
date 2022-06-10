/*************************************************************************
> FileName: str-split.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月03日 星期五 23时32分19秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
int main (void)
{
    char str[] = "mv a.c b.c";
    char *p;
    char *buff;
    buff=str;
    p = strsep (&buff, " ");
    while (p != NULL) {
        printf ("%s\n", p);
        p = strsep (&buff, " ");
    }

    return 0;
}
