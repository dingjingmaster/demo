/******************************************************************
> FileName: gio-all-mount.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu Jan  9 11:46:26 2025
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* read_line (FILE* fr);

int main (int argc, char* argv[])
{
	FILE* fr = fopen("/etc/mtab", "r");
    char* bufLine = NULL;
    while ((bufLine = read_line(fr)) != NULL) {
		if (bufLine && bufLine[0] == '/') {
			printf ("%s\n", bufLine);
		}
        free (bufLine);
        bufLine = NULL;
    }
    fclose(fr);

    if (bufLine) {
        free (bufLine);
    }

    return 0;
}

static inline char* calloc_and_memcpy(const char* oldStr, int64_t oldLen, const char* addStr, int64_t addLen)
{
    int64_t newLineLen = oldLen + addLen;
    char* tmp = (char*) malloc (newLineLen + 1);
    if (!tmp) {
        return NULL;
    }

    memset(tmp, 0, newLineLen + 1);
    if (oldStr) {
        memcpy(tmp, oldStr, oldLen);
    }
    memcpy(tmp + oldLen, addStr, addLen);

    return tmp;
}

char* read_line (FILE* fr)
{
    if (!fr) {
        printf("fopen error\n");
        return NULL;
    }
    int64_t cur = ftell(fr);

    char* res = NULL;
    int64_t lineLen = 0;

    char buf[10] = {0};
    while (true) {
        memset(buf, 0, sizeof(buf));
        int size = fread(buf, 1, sizeof(buf) - 1, fr);
        if (size <= 0) {
            break;
        }

        // 截取一行
        int i = 0;
        bool find = false;
        for (i = 0; i < size; ++i) {
            if (buf[i] == '\n') {
                // 找到
                char* tmp = calloc_and_memcpy(res, lineLen, buf, i);
                if (res) { free (res); res = NULL; }
                if (!tmp) { find = true; printf("impossible\n"); break; }           // impossible
                res = tmp;
                lineLen += i;
                find = true;
                break;
            }
        }

        if (find) { fseek(fr, cur + lineLen + 1, SEEK_SET); break; }

        // 分配内存, 一行的长度大于 buf
        char* tmp = calloc_and_memcpy(res, lineLen, buf, size);
        if (res) { free(res); res = NULL; }
        if (!tmp) { find = true; printf("impossible\n"); break; } // impossible
        res = tmp;
        lineLen += size;
    }

    return res;
}
