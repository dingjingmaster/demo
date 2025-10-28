/*************************************************************************
> FileName: file-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年10月28日 星期二 16时31分26秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>


int safe_utils_get_file_name_and_dir (const uint8_t* filePath, uint8_t* fileName, uint32_t fileNameLen, uint8_t* dirPath, uint32_t dirPathLen)
{
    int i = 0;
    int j = 0;
    int nameLen = 0;
    const uint8_t* name = NULL;

    if (!filePath || !fileName || !dirPath) { return -1; }

    memset(fileName, 0, fileNameLen);
    memset(dirPath, 0, dirPathLen);

    nameLen = (int) strlen ((char*) filePath);

    for (i = nameLen - 1; ((i >= 0) && ('/' != filePath[i])); name = filePath + i, i--);

    if (!name) { name = filePath; }

    for (i = 0; i < nameLen; i++) {
        if (filePath + i < name) {
            if (i < dirPathLen - 1) {
                dirPath[i] = (uint8_t) filePath[i];
            }
            continue;
        }
        j = (int) (filePath - name) + i;
        if (j < fileNameLen - 1) {
            fileName[j] = (uint8_t) filePath[i];
        }
    }

    return 0;
}


int main (int argc, char* argv[])
{
    if (2 != argc) {
        printf ("%s <file>\n", argv[0]);
        return -1;
    }

    char fileDir[10] = {0};
    char fileName[10] = {0};
    const char* filePath = argv[1];

    if (0 != safe_utils_get_file_name_and_dir(filePath, fileName, sizeof(fileName), fileDir, sizeof(fileDir))) {
        printf ("error\n");
        return -2;
    }

    printf ("name: %s\n", fileName);
    printf ("dir: %s\n", fileDir);

    return 0;
}
