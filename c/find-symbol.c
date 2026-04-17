/*************************************************************************
> FileName: find-symbol.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年03月26日 星期四 15时04分30秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
    FILE* f = NULL;
    int symLen = 0;
    char* sym = "sys_call_table";
    char buf[2048] = {0};


    f = fopen("/proc/kallsyms", "r");

    do {
        char t = '0';
        int idx = 0;
        while ((fread(&t, 1, 1, f)) >= 1) {
            if (t != '\n') {
                buf[idx] = t;
                idx++;
                continue;
            }
            else {
                char* pos = NULL;
                if (pos = strstr(buf, sym)) {
                    printf ("%s\n", buf);
                }
                memset(buf, 0, sizeof(buf));
                idx = 0;
            }
        }
    } while (0);

    fclose (f);

    return 0;
}

