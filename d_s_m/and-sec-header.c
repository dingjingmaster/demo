#include <stdio.h>

#include "and-sec-header.h"


int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要读取的加密文件\n");
        return -1;
    }

    printf("size: %d\n", sizeof(SafeFileHead));
    printf("size: %d\n", sizeof(DrmFileHead));

    return 0;
}
