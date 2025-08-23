#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


bool is_file_opened (const char* filePath)
{
    DIR* procDir = opendir("/proc");

    if (!procDir) { return false; }

    char fdPath[128] = {0};
    char linkPath[128] = {0};
    char linkFilePath[PATH_MAX] = {0};

    struct dirent* procDirEntry = NULL;

    while (NULL != (procDirEntry = readdir(procDir))) {
        if (DT_DIR != procDirEntry->d_type) {
            continue;
        }
        if (!isdigit(procDirEntry->d_name[0])) {
            continue;
        }
        memset(fdPath, 0, sizeof(fdPath));
        snprintf(fdPath, sizeof(fdPath) - 1, "/proc/%s/fd", procDirEntry->d_name);
        DIR* fdDir = opendir(fdPath);
        if (!fdDir) { continue; }

        struct dirent* fdEntry = NULL;
        while (NULL != (fdEntry = readdir(fdDir))) {
            if (fdEntry->d_name[0] == '.') {
                continue;
            }
            memset(linkPath, 0, sizeof(linkPath));
            memset(linkFilePath, 0, sizeof(linkFilePath));
            snprintf(linkPath, sizeof(linkPath) - 1, "%s/%s", fdPath, fdEntry->d_name);
            if (-1 != readlink(linkPath, linkFilePath, sizeof(linkFilePath) - 1)) {
                if (0 == strcmp(filePath, linkFilePath)) {
                    closedir(fdDir);
                    closedir(procDir);
                    return true;
                }
            }
        }
        closedir(fdDir);
    }

    closedir(procDir);

    return false;
}


int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf("%s <file>\n", argv[0]);
        return 1;
    }

    printf("file: %s, open: %s\n", argv[1], is_file_opened(argv[1]) ? "true" : "false");

    return 0;
}
