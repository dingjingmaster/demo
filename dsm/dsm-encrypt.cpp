/*************************************************************************
> FileName: dsm-encrypt.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 29 Mar 2023 02:21:45 PM CST
 ************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include "common.h"

int main (int argc, char* argv[])
{
    if (argc < 3) {
        printf ("请输入要加密的文件 和 密钥\n");
        return -1;
    }

    //g_autofree char* str = g_strdup_printf("cp %s ./encode-file.txt", argv[1]);
    //printf ("%s\n", str);
    //system (str);

    if (EncryptLockFile((char*) argv[1], (unsigned char*) argv[2], 2)) {
        printf ("加密成功, 密文输出: ./encode-file.txt\n");
        return 0;
    }
    else {
        printf ("加密失败\n");
    }


    return 0;
}
