/*************************************************************************
> FileName: env.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 15时21分44秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENV_BUF_SIZE 65536

int main() {
    FILE *fp = fopen("/proc/self/environ", "rb");
    if (!fp) {
        perror("无法打开 /proc/self/environ");
        return 1;
    }

    char *buf = malloc(ENV_BUF_SIZE);
    if (!buf) {
        perror("内存分配失败");
        fclose(fp);
        return 1;
    }

    size_t len = fread(buf, 1, ENV_BUF_SIZE - 1, fp);
    fclose(fp);
    buf[len] = '\0';

    char *p = buf;
    while (p < buf + len) {
        char *eq = strchr(p, '=');
        if (eq) {
            *eq = '\0';
            char *val = eq + 1;
            char *end = strchr(val, '\0');
            printf("键: '%s', 值: '%s'\n", p, val);
            p = end + 1;
        } else {
            break;
        }
    }

    free(buf);
    return 0;
}
