#include <stdio.h>

#include "and-sec-header.h"


#define BOOL_STR(x) (x ? "true" : "false")


int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要读取的加密文件\n");
        return -1;
    }

    SafeFileHead sh;

    bool res = ReadSafeHead(argv[1], &sh);
    printf("ReadSafeHead: %s\n", BOOL_STR(res));

    res = CheckSafeHead(&sh, FILE_TYPE_ANY);
    printf("CheckSafeHead: %s\n", BOOL_STR(res));
    if (!res) {
        return -1;
    }

    print_safe_head(&sh);

    return 0;
}
