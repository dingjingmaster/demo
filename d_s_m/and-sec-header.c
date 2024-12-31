#include <stdio.h>
#include <string.h>

#include "and-sec-header.h"


#define BOOL_STR(x) (x ? "true" : "false")

#define MASTER_KEY "fa49c7b67bab98daa75f680990ac03b126d640495b900715" //"ZmE0OWM3YjY3YmFiOThkYWE3NWY2ODA5OTBhYzAzYjEyNmQ2NDA0OTViOTAwNzE1"


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

#if 0
    DrmFileHead drmFileHead;
    FILE* fr = fopen(argv[1], "r");
    if (fr) {
        size_t readSize = fread(&drmFileHead, sizeof(drmFileHead), 1, fr);
        printf("read size: %lu\n", readSize);
        fclose(fr);
    }
#endif

    char key[32] = {0};
    memcpy(key, MASTER_KEY, sizeof(key));

    int ret = DecryptSafeFile(argv[1], MASTER_KEY, ENCRYPT_ARITH_RC4);
    printf("DecodeDRMHead: %d\n", ret);

    print_safe_head(&sh);

    return 0;
}
