#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hs.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#define MAX_THREADS 8
#define MIN_CHUNK_SIZE (1024 * 1024)  // 1MB最小块

// 线程数据结构
typedef struct {
    int thread_id;
    const char* data;
    size_t offset;
    size_t size;
    hs_database_t* database;
    int match_count;
} thread_data_t;

// 全局变量
static pthread_mutex_t match_mutex = PTHREAD_MUTEX_INITIALIZER;
static int total_matches = 0;

// 匹配回调函数
static int match_handler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
    thread_data_t* thread_data = (thread_data_t*)ctx;
    
    pthread_mutex_lock(&match_mutex);
    total_matches++;
    thread_data->match_count++;
    
    printf("线程%d: 匹配到模式 ID=%u, 位置=%llu-%llu (全局位置=%llu-%llu)\n", 
           thread_data->thread_id, id, from, to, 
           from + thread_data->offset, to + thread_data->offset);
    
    pthread_mutex_unlock(&match_mutex);
    return 0;
}

// 线程工作函数
void* worker_thread(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    // 分配临时空间
    hs_scratch_t* scratch;
    if (hs_alloc_scratch(data->database, &scratch) != HS_SUCCESS) {
        printf("线程%d: 分配临时空间失败\n", data->thread_id);
        return NULL;
    }
    
    printf("线程%d: 开始扫描块 (偏移=%zu, 大小=%zu)\n", 
           data->thread_id, data->offset, data->size);
    
    clock_t start = clock();
    
    // 执行扫描
    if (hs_scan(data->database, data->data, data->size, 0, 
                scratch, match_handler, data) != HS_SUCCESS) {
        printf("线程%d: 扫描失败\n", data->thread_id);
    }
    
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("线程%d: 扫描完成，用时=%.3f秒，匹配=%d个\n", 
           data->thread_id, elapsed, data->match_count);
    
    hs_free_scratch(scratch);
    return NULL;
}

// 多线程扫描文件
int scan_file_multithread(const char* filename, hs_database_t* database, int num_threads) {
    printf("使用%d个线程扫描文件...\n", num_threads);
    
    // 打开文件
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
    if (file_size < MIN_CHUNK_SIZE) {
        printf("文件太小，使用单线程模式\n");
        num_threads = 1;
    }
    
    // 内存映射文件
    char* data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("内存映射失败");
        close(fd);
        return -1;
    }
    
    close(fd);
    
    // 计算每个线程的数据块大小
    size_t chunk_size = file_size / num_threads;
    if (chunk_size < MIN_CHUNK_SIZE && num_threads > 1) {
        chunk_size = MIN_CHUNK_SIZE;
        num_threads = (file_size / MIN_CHUNK_SIZE) + (file_size % MIN_CHUNK_SIZE ? 1 : 0);
        if (num_threads > MAX_THREADS) num_threads = MAX_THREADS;
    }
    
    printf("文件大小: %.2f MB, 线程数: %d, 块大小: %.2f MB\n", 
           file_size / (1024.0 * 1024.0), num_threads, 
           chunk_size / (1024.0 * 1024.0));
    
    // 创建线程数据
    thread_data_t* thread_data = malloc(num_threads * sizeof(thread_data_t));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    
    clock_t start_time = clock();
    
    // 创建线程
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].database = database;
        thread_data[i].offset = i * chunk_size;
        thread_data[i].match_count = 0;
        
        if (i == num_threads - 1) {
            // 最后一个线程处理剩余数据
            thread_data[i].size = file_size - thread_data[i].offset;
        } else {
            thread_data[i].size = chunk_size;
        }
        
        thread_data[i].data = data + thread_data[i].offset;
        
        if (pthread_create(&threads[i], NULL, worker_thread, &thread_data[i]) != 0) {
            printf("创建线程%d失败\n", i);
            // 清理已创建的线程
            for (int j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }
            free(thread_data);
            free(threads);
            munmap(data, file_size);
            return -1;
        }
    }
    
    // 等待所有线程完成
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_t end_time = clock();
    double elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== 多线程扫描结果 ===\n");
    printf("总扫描时间: %.3f 秒\n", elapsed);
    printf("总匹配数: %d\n", total_matches);
    printf("扫描速度: %.2f MB/s\n", (file_size / (1024.0 * 1024.0)) / elapsed);
    
    printf("\n各线程匹配统计:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("  线程%d: %d 个匹配\n", i, thread_data[i].match_count);
    }
    
    // 清理资源
    free(thread_data);
    free(threads);
    munmap(data, file_size);
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("用法: %s <文件路径> [线程数]\n", argv[0]);
        printf("  线程数: 1-%d (默认为CPU核心数)\n", MAX_THREADS);
        return 1;
    }
    
    const char* filename = argv[1];
    int num_threads = (argc > 2) ? atoi(argv[2]) : sysconf(_SC_NPROCESSORS_ONLN);
    
    if (num_threads < 1) num_threads = 1;
    if (num_threads > MAX_THREADS) num_threads = MAX_THREADS;
    
    // 定义搜索模式
    const char* patterns[] = {
        "error|错误",
        "warning|警告",
        "failed|失败",
        "success|成功",
        "\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b", // 邮箱
        "\\b(?:[0-9]{1,3}\\.){3}[0-9]{1,3}\\b", // IP地址
        "https?://[^\\s]+",      // URL
        "\\b\\d{3}-\\d{2}-\\d{4}\\b", // SSN
    };
    
    unsigned int flags[] = {
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        HS_FLAG_CASELESS,
        0,
        HS_FLAG_CASELESS,
        0
    };
    
    unsigned int ids[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int pattern_count = sizeof(patterns) / sizeof(patterns[0]);
    
    // 编译HyperScan数据库
    hs_database_t* database;
    hs_compile_error_t* compile_err;
    
    printf("编译HyperScan数据库 (%d个模式)...\n", pattern_count);
    if (hs_compile_multi(patterns, flags, ids, pattern_count,
                        HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
        printf("编译失败: %s\n", compile_err->message);
        hs_free_compile_error(compile_err);
        return 1;
    }
    
    printf("数据库编译成功\n");
    
    // 执行多线程扫描
    total_matches = 0;
    int result = scan_file_multithread(filename, database, num_threads);
    
    // 清理数据库
    hs_free_database(database);
    
    return result;
} 