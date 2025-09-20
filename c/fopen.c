/*************************************************************************
> FileName: fopen.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月20日 星期六 11时58分28秒
 ************************************************************************/
#include <stdio.h>

int main (void)
{
    char path[1024] = {0};
    for (int i = 0; i < 100000; ++i) {
        snprintf(path, sizeof(path) - 1, "/tmp/fopen-%d", i);
        FILE* f = fopen(path, "w+");
        if (!f) {
            printf ("err\n");
        }
    }

    return 0;
}
