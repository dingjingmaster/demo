#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hs.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

// 匹配回调函数
static int match_handler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
    printf("匹配到关键词: ID=%u, 位置=%llu-%llu\n", id, from, to);
    return 0; // 继续扫描
}

// 读取文件内容
char* read_file(const char* filename, size_t* file_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("打开文件失败");
        return NULL;
    }
    
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("获取文件状态失败");
        close(fd);
        return NULL;
    }
    
    *file_size = st.st_size;
    
    // 对于大文件，使用mmap提高效率
    char* data = mmap(NULL, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("内存映射失败");
        close(fd);
        return NULL;
    }
    
    close(fd);
    return data;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("用法: %s <文件路径>\n", argv[0]);
        return 1;
    }
    
    // 定义要匹配的关键词模式
    const char* patterns[] = {
        "error",       // 错误信息
        "warning",     // 警告信息
        "failed",      // 失败信息
        "success",     // 成功信息
        "\\b\\d{3}-\\d{2}-\\d{4}\\b",  // 社会安全号码格式
        "\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b", // 邮箱格式
    };
    
    unsigned int pattern_flags[] = {
        HS_FLAG_CASELESS,    // 忽略大小写
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        0,                   // 正则表达式，区分大小写
        HS_FLAG_CASELESS,    // 邮箱匹配
    };
    
    unsigned int pattern_ids[] = {1, 2, 3, 4, 5, 6};
    
    int pattern_count = sizeof(patterns) / sizeof(patterns[0]);
    
    // 编译HyperScan数据库
    hs_database_t* database;
    hs_compile_error_t* compile_err;
    
    printf("编译HyperScan数据库...\n");
    if (hs_compile_multi(patterns, pattern_flags, pattern_ids, pattern_count,
                        HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
        printf("编译失败: %s\n", compile_err->message);
        hs_free_compile_error(compile_err);
        return 1;
    }
    
    printf("HyperScan数据库编译成功\n");
    
    // 分配临时空间
    size_t scratch_size;
    hs_scratch_t* scratch;
    
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        printf("分配临时空间失败\n");
        hs_free_database(database);
        return 1;
    }
    
    // 读取文件
    size_t file_size;
    char* file_data = read_file(argv[1], &file_size);
    if (!file_data) {
        hs_free_scratch(scratch);
        hs_free_database(database);
        return 1;
    }
    
    printf("正在扫描文件: %s (大小: %zu 字节)\n", argv[1], file_size);
    
    // 执行匹配
    if (hs_scan(database, file_data, file_size, 0, scratch, match_handler, NULL) != HS_SUCCESS) {
        printf("扫描失败\n");
    } else {
        printf("扫描完成\n");
    }
    
    // 清理资源
    munmap(file_data, file_size);
    hs_free_scratch(scratch);
    hs_free_database(database);
    
    return 0;
} 