/*************************************************************************
> FileName: file-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月28日 星期二 16时31分26秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>


int main (int argc, char* argv[])
{
    if (2 != argc) {
        printf ("%s <file>\n", argv[0]);
        return -1;
    }

    char* name = NULL;
    char fileDir[4096] = {0};
    const char* filePath = argv[1];
    int nameLen = strlen(filePath);

    for (int i = nameLen - 1; ((i >= 0) && ('/' != filePath[i])); name = filePath + i, i--);
    if (!name) {
        name = filePath;
    }

    for (int i = 0; ((filePath + i < name)); fileDir[i] = filePath[i], ++i);

    printf ("name: %s\n", name);
    printf ("dir: %s\n", fileDir);

    return 0;
}
