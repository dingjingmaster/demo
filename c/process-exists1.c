/*************************************************************************
> FileName: process-exists1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月26日 星期一 09时14分52秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>


int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入要查找的进程名\n");
        return -1;
    }

    DIR* dir = opendir("/proc");
    if (NULL == dir) {
        perror("opendir");
        return 1;
    }

    char buf[128] = {0};
    char procPath[PATH_MAX] = {0};
    struct dirent* entry = NULL;
    char bufR[128] = {0};
    snprintf(bufR, sizeof(bufR) - 1, "/%s", argv[1]);
    int len = strlen(bufR);

    while (NULL != (entry = readdir(dir))) {
        int proc = atoi(entry->d_name);
        if (proc <= 0) {
            continue;
        }
        int r = snprintf(buf, sizeof(buf) - 1, "/proc/%d/exe", proc);
        if (r > 0) {
            buf[r] = 0;
            ssize_t t = readlink(buf, procPath, sizeof(procPath) - 1);
            if (t > 0) {
                procPath[t] = 0;
                int len1 = strlen(procPath);
                if ((len1 > len) && (0 == strcasecmp(bufR, &procPath[len1 - len]))) {
                    printf("%d -- %s\n", proc, procPath);
                }
            }
        }
    }
    closedir(dir);


    return 0;
}
