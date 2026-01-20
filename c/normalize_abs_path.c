/*************************************************************************
> FileName: normalize_abs_path.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年01月20日 星期二 14时57分19秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int normalize_abs_path(const char *in, char *out, size_t outsz)
{
    if (!in || in[0] != '/' || !out || outsz == 0)
        return -1;

    const char *p = in;
    size_t len = 0;

    // 栈：保存每一段起始位置
    size_t stack[256];
    int top = 0;

    out[len++] = '/';

    while (*p) {
        while (*p == '/') p++;   // 跳过重复 /

        if (!*p)
            break;

        const char *start = p;
        while (*p && *p != '/') p++;

        size_t seglen = p - start;

        if (seglen == 1 && start[0] == '.') {
            continue;
        }

        if (seglen == 2 && start[0] == '.' && start[1] == '.') {
            if (top > 0) {
                len = stack[--top];
            }
            continue;
        }

        if (len > 1 && len < outsz)
            out[len++] = '/';

        if (len + seglen >= outsz)
            return -1;

        stack[top++] = len;
        memcpy(out + len, start, seglen);
        len += seglen;
    }

    if (len > 1 && out[len - 1] == '/')
        len--;

    out[len] = '\0';
    return 0;
}

int main (int argc, char* argv[])
{
    char buf[256] = {0};
    const char* str1 = "/data/./code/./andsec-dev/././sec_linux/pack/.gitignore";
    const char* str2 = "/data/./code/../source/.././/code/andsec-dev/sec_linux/pack/.gitignore";

    normalize_abs_path(str1, buf, sizeof(buf));
    printf ("'%s' -> '%s'\n", str1, buf);
    if (realpath(str1, buf)) {
        printf (">> '%s' -> '%s'\n", str1, buf);
    }

    normalize_abs_path(str2, buf, sizeof(buf));
    printf ("'%s' -> '%s'\n", str2, buf);
    if (realpath(str2, buf)) {
        printf (">> '%s' -> '%s'\n", str2, buf);
    }

    return 0;
}
