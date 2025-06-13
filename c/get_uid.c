#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main() {
    FILE *fp;
    char line[256];
    uid_t uid = 0;
    
    // 打开 /proc/self/status 文件
    fp = fopen("/proc/self/status", "r");
    if (fp == NULL) {
        perror("无法打开 /proc/self/status");
        return 1;
    }
    
    // 逐行读取文件内容
    while (fgets(line, sizeof(line), fp)) {
        // 查找包含 "Uid:" 的行
        if (strncmp(line, "Uid:", 4) == 0) {
            // 解析 UID 值
            sscanf(line, "Uid:\t%u", &uid);
            break;
        }
    }
    
    // 关闭文件
    fclose(fp);
    
    // 打印结果
    printf("进程 UID: %u\n", uid);
    
    return 0;
} 