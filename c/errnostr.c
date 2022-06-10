/*************************************************************************
> FileName: errnostr.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2022年01月24日 星期一 15时19分21秒
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    for (int i = 0; i < 500; ++i) {
        const char* err = strerror(i);
        if (err) {
            printf ("%d --- %s\n", i, err);
        }
    }

    return 0;
}
