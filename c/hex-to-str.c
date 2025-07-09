/*************************************************************************
> FileName: hex-to-str.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年07月09日 星期三 14时02分11秒
 ************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


int hex_char_to_int (char c)
{
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;

    printf ("hex_char_to_int error!\n");
    return -1;
}

char* hex_to_ascii_or_oct (const char* hexStr)
{
    size_t len = strlen(hexStr);
    if (len % 2 != 0) return NULL;

    size_t maxOutput = len * 2 + 1;
    char* result = malloc (maxOutput);
    if (!result) { printf ("malloc error!\n"); return NULL;}

    size_t outI = 0;
    for (size_t i = 0; i < len; i += 2) {
        int hi = hex_char_to_int (hexStr[i]);
        int lo = hex_char_to_int (hexStr[i + 1]);
        if (-1 == hi || -1 == lo) {
            free (result);
            return NULL;
        }
        unsigned char byte = (hi << 4) | lo;
        if (isprint(byte)) {
            result[outI++] = byte;
        }
        else {
            sprintf(result + outI, "\\%03o", byte);
            outI += 4;
        }
    }
    result[outI] = '\0';

    return result;
}

void format_str (char* str)
{
    int len = strlen(str);

    printf ("format 1: %s\n", str);
    
    for (int i = 0; i < len && str[i]; ++i) {
        if (str[i] == ' ') {
            for (int j = i; j < len && str[j]; ++j) {
                str[j] = str[j + 1];
            }
        }
    }

    printf ("format 2: %s\n", str);
}

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("输入要转换的十六进制串\n");
        return -1;
    }

    format_str(argv[1]);

    char* output = hex_to_ascii_or_oct (argv[1]);
    if (output) {
        printf("%s\n", output);
        free (output);
    }
    else {
        printf ("error!\n");
    }

    return 0;
}

