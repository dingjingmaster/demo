#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

#define CGROUP_PATH "/sys/fs/cgroup/my_cgroup"

// 创建 cgroup 控制组并将主进程添加到该组
int create_cgroup() {
    // 创建 cgroup 目录
    if (mkdir(CGROUP_PATH, 0755) && errno != EEXIST) {
        perror("Failed to create cgroup");
        return -1;
    }

    // 将当前进程写入 cgroup.procs 文件
    int fd = open(CGROUP_PATH "/cgroup.procs", O_WRONLY);
    if (fd == -1) {
        perror("Failed to open cgroup.procs");
        return -1;
    }

    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", getpid());
    if (write(fd, pid_str, strlen(pid_str)) == -1) {
        perror("Failed to write to cgroup.procs");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

// 监控 cgroup.procs 文件，查看新创建的进程
void monitor_cgroup() {
    int fd = open(CGROUP_PATH "/cgroup.procs", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open cgroup.procs for monitoring");
        return;
    }

    char buffer[256];
    while (1) {
        lseek(fd, 0, SEEK_SET); // 将文件指针移到文件开始
        int len = read(fd, buffer, sizeof(buffer) - 1);
        if (len == -1) {
            perror("Failed to read cgroup.procs");
            break;
        }

        buffer[len] = '\0';
        printf("Current processes in cgroup:\n%s", buffer);
        
        sleep(2); // 设置监控频率
    }

    close(fd);
}

int main() {
    if (create_cgroup() == -1) {
        fprintf(stderr, "Error creating cgroup\n");
        return EXIT_FAILURE;
    }

    printf("Monitoring cgroup for new processes...\n");
    monitor_cgroup();

    // 退出时删除 cgroup
    if (rmdir(CGROUP_PATH) == -1) {
        perror("Failed to remove cgroup directory");
    }

    return EXIT_SUCCESS;
}
