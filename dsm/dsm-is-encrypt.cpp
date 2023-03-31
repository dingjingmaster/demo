/*************************************************************************
> FileName: dsm-is-encrypt.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 31 Mar 2023 08:24:29 AM CST
 ************************************************************************/
#include <stdio.h>
#include "common.h"

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要检查的文件\n");
        return -1;
    }

    const char* dsmFile = argv[1];

    printf ("Is DSM file: %s\n", (IsEncryptLockFile(dsmFile) ? "true" : "false"));


    return 0;
}
