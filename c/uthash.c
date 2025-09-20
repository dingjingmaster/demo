/*************************************************************************
> FileName: uthash.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月20日 星期六 14时33分05秒
 ************************************************************************/
#include <stdio.h>

#include "uthash.h"

typedef struct 
{
    char* key;
    char* value;

    int k;
    UT_hash_handle hh;
} File;

static File* gsF = NULL;


int main (void)
{
    const char* keys[] = {"apple", "banana", "orange"};
    const char* value[] = {"red", "yellow", "orange"};

    File* item = NULL, *tmp = NULL;
    for (int i = 0; i < 3; ++i) {
        item = malloc(sizeof(File));
        item->key = strdup(keys[i]);
        item->value = strdup(value[i]);
        item->k = i;
        HASH_ADD_KEYPTR(hh, gsF, item->key, strlen(item->key), item);
    }

    // 查找
    HASH_FIND_STR(gsF, "banana", item);
    if (item) {
        printf("key: %s --> %s\n", item->key, item->value);
    }
    else {
        printf ("not found: banana\n");
    }

    // 遍历
    HASH_ITER(hh, gsF, item, tmp) {
        printf(" %s -> %s\n", item->key, item->value);
    }

    // 删除并释放
    HASH_ITER(hh, gsF, item, tmp) {
        HASH_DEL(gsF, item);
        free(item->key);
        free(item->value);
        free(item);
    }

    return 0;
}

