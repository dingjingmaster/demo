/*************************************************************************
> FileName: odg-upper.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年12月08日 星期一 18时29分18秒
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

const char* keyT = "|KEYFLAG_EDG|f071abbf42f5b3937bf70f38a08fcac85d00789ee6b1b484"
               "|KEYFLAG_CDG8|8888888888888888"
               "|xXxx..12XXxx..12"
               "|xXxx1234"
               "|xXxx1234XXxx1234"
               "|xXxxxxxxXXxxxxxxXXxxxxxxXXxxxxxx"
               "|KEYFLAG_EDG"
               "|adasdsadasd|";

void transform_odg_keys (char* keys, int32_t keyLen)
{
    int i = 0;
    bool needTrans = false;

    for (i = 0; i < keyLen; ++i) {
        if (0 == strncmp(keys + i, "|KEYFLAG_", strlen("|KEYFLAG_"))) {
            if (0 == strncmp(keys + i, "|KEYFLAG_CDG8|", strlen("|KEYFLAG_CDG8|"))) {
                needTrans = true;
                i += strlen("|KEYFLAG_CDG8|");
                continue;
            }
            else {
                needTrans = false;
                i += strlen("|KEYFLAG_");
                continue;
            }
        }
        if (needTrans) {
            if (keys[i] >= 'a' && keys[i] <= 'z') {
                keys[i] -= 32;
            }
        }
    }
}

int main (int argc, char* argv[])
{
    char key[4096] = {0};

    snprintf(key, sizeof(key) - 1, "%s", keyT);

    transform_odg_keys(key, strlen(key));

    printf ("orig: %s\ntran: %s\n", keyT, key);

    return 0;
}

