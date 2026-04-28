/*************************************************************************
> FileName: system-is-uos.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年04月28日 星期二 09时48分49秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        if (strcasestr(line, "NAME=") && strcasestr(line, "uos")) {
            printf("Matched line: %s", line);
        }
    }

    fclose(fp);
    return 0;
}
