/*************************************************************************
> FileName: get-pid-by-proc-name.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月16日 星期五 09时03分24秒
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int32_t get_pid_by_proc_name (const char* procName);

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("Usage: %s <proc name>\n", argv[0]);
        return -1;
    }

    int32_t pid = get_pid_by_proc_name (argv[1]);
    if (pid > 0) {
        printf ("%s: %d\n", argv[1], pid);
    }

    return 0;
}

int32_t get_pid_by_proc_name (const char* procName)
{
    if (!procName || access("/proc", F_OK)) { return -1; }

    DIR* dir = opendir("/proc");
    if (!dir) { return -2; }

    struct dirent* entry = NULL;

    int32_t pid = -1;
    char path[64] = {0};
    char proc[256] = {0};
    while ((entry = readdir(dir)) != NULL) {
        if (!atoi(entry->d_name)) {
            continue;
        }

        snprintf(path, sizeof(path) - 1, "/proc/%s/comm", entry->d_name);
        FILE* fr = fopen(path, "r");
        if (!fr) {
            continue;
        }
        memset(proc, 0, sizeof(proc));
        if (fgets(proc, sizeof(proc) - 1, fr)) {
            proc[strcspn(proc, "\n")] = 0;
            if (!strcmp(proc, procName)) {
                pid = atoi(entry->d_name);
                fclose(fr);
                break;
            }
        }
        fclose(fr);
    }
    closedir(dir);

    return pid;
}
