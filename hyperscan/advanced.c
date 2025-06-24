#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hs.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#define CHUNK_SIZE (1024 * 1024)  // 1MB 块大小
#define MAX_PATTERNS 100

// 匹配结果结构
typedef struct {
    unsigned int pattern_id;
    unsigned long long start_pos;
    unsigned long long end_pos;
    char pattern_name[64];
} match_result_t;

// 全局匹配计数器
static int match_count = 0;
static match_result_t matches[10000];

// 模式名称映射
static const char* pattern_names[] = {
    "错误信息",
    "警告信息", 
    "失败信息",
    "成功信息",
    "社会安全号码",
    "邮箱地址",
    "IP地址",
    "URL链接",
    "信用卡号",
    "电话号码"
};

// 匹配回调函数
static int match_handler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
    if (match_count < 10000) {
        matches[match_count].pattern_id = id;
        matches[match_count].start_pos = from;
        matches[match_count].end_pos = to;
        
        if (id <= 10) {
            strcpy(matches[match_count].pattern_name, pattern_names[id-1]);
        } else {
            sprintf(matches[match_count].pattern_name, "模式_%u", id);
        }
        
        match_count++;
    }
    return 0; // 继续扫描
}

// 打印匹配统计信息
void print_match_statistics() {
    printf("\n=== 匹配统计信息 ===\n");
    printf("总匹配次数: %d\n", match_count);
    
    // 按模式分组统计
    int pattern_counts[MAX_PATTERNS] = {0};
    for (int i = 0; i < match_count; i++) {
        if (matches[i].pattern_id < MAX_PATTERNS) {
            pattern_counts[matches[i].pattern_id]++;
        }
    }
    
    printf("\n各模式匹配次数:\n");
    for (int i = 1; i <= 10; i++) {
        if (pattern_counts[i] > 0) {
            printf("  %s: %d 次\n", pattern_names[i-1], pattern_counts[i]);
        }
    }
}

// 块模式扫描
int scan_file_block_mode(const char* filename, hs_database_t* database) {
    printf("使用块模式扫描文件...\n");
    
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("打开文件失败");
        return -1;
    }
    
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("获取文件状态失败");
        close(fd);
        return -1;
    }
    
    size_t file_size = st.st_size;
    
    // 使用mmap映射整个文件
    char* data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("内存映射失败");
        close(fd);
        return -1;
    }
    
    // 分配临时空间
    hs_scratch_t* scratch;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        printf("分配临时空间失败\n");
        munmap(data, file_size);
        close(fd);
        return -1;
    }
    
    clock_t start_time = clock();
    
    // 执行匹配
    if (hs_scan(database, data, file_size, 0, scratch, match_handler, NULL) != HS_SUCCESS) {
        printf("扫描失败\n");
        hs_free_scratch(scratch);
        munmap(data, file_size);
        close(fd);
        return -1;
    }
    
    clock_t end_time = clock();
    double elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("块模式扫描完成，用时: %.3f 秒\n", elapsed);
    printf("扫描速度: %.2f MB/s\n", (file_size / (1024.0 * 1024.0)) / elapsed);
    
    // 清理资源
    hs_free_scratch(scratch);
    munmap(data, file_size);
    close(fd);
    
    return 0;
}

// 流模式扫描（适合超大文件）
int scan_file_stream_mode(const char* filename, hs_database_t* database) {
    printf("使用流模式扫描文件...\n");
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("打开文件失败");
        return -1;
    }
    
    // 分配临时空间
    hs_scratch_t* scratch;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        printf("分配临时空间失败\n");
        fclose(file);
        return -1;
    }
    
    // 创建流
    hs_stream_t* stream;
    if (hs_open_stream(database, 0, &stream) != HS_SUCCESS) {
        printf("创建流失败\n");
        hs_free_scratch(scratch);
        fclose(file);
        return -1;
    }
    
    clock_t start_time = clock();
    char* buffer = malloc(CHUNK_SIZE);
    size_t total_bytes = 0;
    
    // 分块读取和扫描
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, CHUNK_SIZE, file);
        if (bytes_read > 0) {
            total_bytes += bytes_read;
            
            if (hs_scan_stream(stream, buffer, bytes_read, 0, scratch, 
                              match_handler, NULL) != HS_SUCCESS) {
                printf("流扫描失败\n");
                break;
            }
        }
    }
    
    // 关闭流
    if (hs_close_stream(stream, scratch, match_handler, NULL) != HS_SUCCESS) {
        printf("关闭流失败\n");
    }
    
    clock_t end_time = clock();
    double elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("流模式扫描完成，用时: %.3f 秒\n", elapsed);
    printf("扫描速度: %.2f MB/s\n", (total_bytes / (1024.0 * 1024.0)) / elapsed);
    
    // 清理资源
    free(buffer);
    hs_free_scratch(scratch);
    fclose(file);
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("用法: %s <文件路径> [模式:block|stream]\n", argv[0]);
        printf("  block:  块模式 (适合中小型文件)\n");
        printf("  stream: 流模式 (适合超大文件)\n");
        return 1;
    }
    
    const char* filename = argv[1];
    const char* mode = (argc > 2) ? argv[2] : "block";
    
    // 定义要匹配的关键词和正则表达式模式
    const char* patterns[] = {
        "error|错误|失败",                    // 错误信息
        "warning|warn|警告|注意",             // 警告信息
        "failed|failure|失败",               // 失败信息
        "success|成功|完成",                 // 成功信息
        "\\b\\d{3}-\\d{2}-\\d{4}\\b",       // 社会安全号码
        "\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b", // 邮箱
        "\\b(?:[0-9]{1,3}\\.){3}[0-9]{1,3}\\b", // IP地址
        "https?://[^\\s]+",                  // URL链接
        "\\b(?:4[0-9]{12}(?:[0-9]{3})?|5[1-5][0-9]{14}|3[47][0-9]{13})\\b", // 信用卡号
        "\\b\\d{3}-\\d{3}-\\d{4}\\b"        // 电话号码
    };
    
    unsigned int pattern_flags[] = {
        HS_FLAG_CASELESS,    // 忽略大小写
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        0,                   // 正则表达式
        HS_FLAG_CASELESS,
        0,                   // IP地址
        HS_FLAG_CASELESS,    // URL
        0,                   // 信用卡号
        0                    // 电话号码
    };
    
    unsigned int pattern_ids[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int pattern_count = sizeof(patterns) / sizeof(patterns[0]);
    
    // 编译HyperScan数据库
    hs_database_t* database;
    hs_compile_error_t* compile_err;
    
    printf("编译HyperScan数据库 (%d 个模式)...\n", pattern_count);
    
    unsigned int mode_flag = (strcmp(mode, "stream") == 0) ? HS_MODE_STREAM : HS_MODE_BLOCK;
    
    if (hs_compile_multi(patterns, pattern_flags, pattern_ids, pattern_count,
                        mode_flag, NULL, &database, &compile_err) != HS_SUCCESS) {
        printf("编译失败: %s\n", compile_err->message);
        hs_free_compile_error(compile_err);
        return 1;
    }
    
    printf("HyperScan数据库编译成功\n");
    
    // 获取文件大小
    struct stat st;
    if (stat(filename, &st) == 0) {
        printf("文件大小: %.2f MB\n", st.st_size / (1024.0 * 1024.0));
    }
    
    // 根据模式执行扫描
    match_count = 0;
    int result;
    
    if (strcmp(mode, "stream") == 0) {
        result = scan_file_stream_mode(filename, database);
    } else {
        result = scan_file_block_mode(filename, database);
    }
    
    if (result == 0) {
        print_match_statistics();
        
        // 显示前20个匹配结果的详细信息
        printf("\n=== 匹配结果详情 (前20条) ===\n");
        int display_count = (match_count > 20) ? 20 : match_count;
        for (int i = 0; i < display_count; i++) {
            printf("%d. %s: 位置 %llu-%llu\n", 
                   i+1, matches[i].pattern_name, 
                   matches[i].start_pos, matches[i].end_pos);
        }
        
        if (match_count > 20) {
            printf("... 还有 %d 条匹配结果\n", match_count - 20);
        }
    }
    
    // 清理数据库
    hs_free_database(database);
    
    return result;
} 