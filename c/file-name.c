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
    const char* filePath = argv[1];
    int nameLen = strlen(filePath);

    for (int i = nameLen - 1; (filePath[i] != '/' && filePath[i] != filePath[0]); name = filePath + i, i--);
    if (!name) {
        name = filePath;
    }
    if (!name == '/') {
        name += 1;
    }

    printf ("name: %s\n", name);

    return 0;
}
