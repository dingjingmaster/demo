#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PATH_LENGTH 512

/**
 * 检查路径是否为动态库
 * @param path 文件路径
 * @return 1 如果是动态库，0 如果不是
 */
int is_dynamic_library(const char *path) {
    if (!path || strlen(path) == 0) {
        return 0;
    }
    
    // 检查是否以 .so 结尾
    const char *ext = strrchr(path, '.');
    if (ext && strcmp(ext, ".so") == 0) {
        return 1;
    }
    
    // 检查是否包含 .so. (版本号的动态库)
    if (strstr(path, ".so.") != NULL) {
        return 1;
    }
    
    return 0;
}

/**
 * 检查路径是否已经在列表中
 * @param libraries 动态库路径数组
 * @param count 当前数量
 * @param path 要检查的路径
 * @return 1 如果已存在，0 如果不存在
 */
int path_exists(char libraries[][MAX_PATH_LENGTH], int count, const char *path) {
    for (int i = 0; i < count; i++) {
        if (strcmp(libraries[i], path) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * 解析 /proc/self/maps 文件获取动态库路径
 * @param libraries 用于存储动态库路径的数组
 * @param max_count 最大数量
 * @return 找到的动态库数量
 */
int parse_proc_maps(char libraries[][MAX_PATH_LENGTH], int max_count) {
    FILE *file = fopen("/proc/self/maps", "r");
    if (!file) {
        perror("无法打开 /proc/self/maps");
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("正在解析 /proc/self/maps 文件...\n\n");
    
    while (fgets(line, sizeof(line), file) && count < max_count) {
        // 移除换行符
        line[strcspn(line, "\n")] = 0;
        
        // /proc/self/maps 格式：
        // 地址范围 权限 偏移量 设备 inode 路径名
        // 例如：7f1234567000-7f1234568000 r-xp 00000000 08:01 1234567 /lib/x86_64-linux-gnu/libc.so.6
        
        char *tokens[6];
        char *token = strtok(line, " ");
        int token_count = 0;
        
        // 分割行内容
        while (token && token_count < 6) {
            tokens[token_count++] = token;
            token = strtok(NULL, " ");
        }
        
        // 如果有路径字段（第6个字段）
        if (token_count >= 6) {
            char *path = tokens[5];
            
            // 跳过剩余的空格，获取完整路径
            while (*path == ' ') path++;
            
            // 检查是否为动态库且不重复
            if (is_dynamic_library(path) && !path_exists(libraries, count, path)) {
                strncpy(libraries[count], path, MAX_PATH_LENGTH - 1);
                libraries[count][MAX_PATH_LENGTH - 1] = '\0';
                count++;
                printf("发现动态库 [%d]: %s\n", count, path);
            }
        }
    }
    
    fclose(file);
    return count;
}

/**
 * 打印所有找到的动态库
 */
void print_libraries(char libraries[][MAX_PATH_LENGTH], int count) {
    printf("\n==========================================\n");
    printf("总共找到 %d 个动态库:\n", count);
    printf("==========================================\n");
    
    for (int i = 0; i < count; i++) {
        printf("%3d. %s\n", i + 1, libraries[i]);
    }
    printf("==========================================\n");
}

/**
 * 分析动态库统计信息
 */
void analyze_libraries(char libraries[][MAX_PATH_LENGTH], int count) {
    printf("\n动态库统计分析:\n");
    printf("------------------------------------------\n");
    
    int system_libs = 0;      // 系统库 (/lib, /usr/lib)
    int local_libs = 0;       // 本地库 (./当前目录)
    int other_libs = 0;       // 其他位置
    
    for (int i = 0; i < count; i++) {
        if (strncmp(libraries[i], "/lib", 4) == 0 || 
            strncmp(libraries[i], "/usr/lib", 8) == 0) {
            system_libs++;
        } else if (strncmp(libraries[i], "./", 2) == 0 || 
                   libraries[i][0] != '/') {
            local_libs++;
        } else {
            other_libs++;
        }
    }
    
    printf("系统库数量: %d\n", system_libs);
    printf("本地库数量: %d\n", local_libs);
    printf("其他库数量: %d\n", other_libs);
    printf("------------------------------------------\n");
}

int main() {
    const int MAX_LIBRARIES = 100;
    char libraries[MAX_LIBRARIES][MAX_PATH_LENGTH];
    
    printf("=== Linux 动态库路径解析工具 ===\n");
    printf("当前进程 PID: %d\n", getpid());
    printf("解析文件: /proc/self/maps\n\n");
    
    // 解析动态库
    int count = parse_proc_maps(libraries, MAX_LIBRARIES);
    
    if (count < 0) {
        fprintf(stderr, "解析失败\n");
        return 1;
    }
    
    if (count == 0) {
        printf("未找到任何动态库\n");
        return 0;
    }
    
    // 打印结果
    print_libraries(libraries, count);
    
    // 统计分析
    analyze_libraries(libraries, count);
    
    return 0;
} 