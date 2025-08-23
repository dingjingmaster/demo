/*************************************************************************
> FileName: get-child-pid.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年08月23日 星期六 09时16分14秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define PROC_DIR "/proc"

int main() 
{
    pid_t self_pid = getpid();
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(PROC_DIR)) == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char *endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr != '\0') {
                continue; // Not a PID directory
            }

            char stat_path[256];
            snprintf(stat_path, sizeof(stat_path), PROC_DIR "/%ld/stat", pid);

            FILE *fp = fopen(stat_path, "r");
            if (fp) {
                int ppid;
                char comm[256], state;
                /*
                 * /proc/[pid]/stat format:
                 * pid (comm) state ppid ...
                 */
                fscanf(fp, "%*d (%[^)]) %c %d", comm, &state, &ppid);
                fclose(fp);

                if (ppid == self_pid) {
                    printf("Child PID: %ld (comm: %s)\n", pid, comm);
                }
            }
        }
    }

    closedir(dir);
    return 0;
}
