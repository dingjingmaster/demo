#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

// Adler 算法的模数常量
#define ADLER16_MOD 251        // 最大的小于 2^8 的质数
#define ADLER32_MOD 65521      // 最大的小于 2^16 的质数  
#define ADLER64_MOD 4294967291UL // 最大的小于 2^32 的质数

/**
 * Adler16 校验和算法
 * @param data 输入数据
 * @param len 数据长度
 * @return 16位校验和
 */
uint16_t adler16(const uint8_t *data, size_t len) {
    uint32_t a = 1, b = 0;
    
    while (len--) {
        a = (a + *data++) % ADLER16_MOD;
        b = (b + a) % ADLER16_MOD;
    }
    
    return (b << 8) | a;
}

/**
 * Adler32 校验和算法 (标准实现)
 * @param data 输入数据
 * @param len 数据长度
 * @return 32位校验和
 */
uint32_t adler32(const uint8_t *data, size_t len) {
    uint32_t a = 1, b = 0;
    
    // 优化：批量处理以减少模运算次数
    while (len > 0) {
        size_t chunk = len > 5552 ? 5552 : len; // 避免溢出的最大块大小
        len -= chunk;
        
        while (chunk--) {
            a += *data++;
            b += a;
        }
        
        a %= ADLER32_MOD;
        b %= ADLER32_MOD;
    }
    
    return (b << 16) | a;
}

/**
 * Adler64 校验和算法
 * @param data 输入数据
 * @param len 数据长度
 * @return 64位校验和
 */
uint64_t adler64(const uint8_t *data, size_t len) {
    uint64_t a = 1, b = 0;
    
    while (len > 0) {
        size_t chunk = len > 380368439 ? 380368439 : len; // 避免溢出的最大块大小
        len -= chunk;
        
        while (chunk--) {
            a += *data++;
            b += a;
        }
        
        a %= ADLER64_MOD;
        b %= ADLER64_MOD;
    }
    
    return (b << 32) | a;
}

/**
 * 计算字符串的所有Adler校验和
 */
void calculate_string_checksums(const char *str) {
    size_t len = strlen(str);
    const uint8_t *data = (const uint8_t *)str;
    
    printf("输入字符串: \"%s\" (长度: %zu)\n", str, len);
    printf("----------------------------------------\n");
    printf("Adler16:  0x%04X (%u)\n", adler16(data, len), adler16(data, len));
    printf("Adler32:  0x%08X (%u)\n", adler32(data, len), adler32(data, len));
    printf("Adler64:  0x%016lX (%lu)\n", adler64(data, len), adler64(data, len));
    printf("\n");
}

/**
 * 计算文件的所有Adler校验和
 */
int calculate_file_checksums(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("打开文件失败");
        return -1;
    }
    
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("获取文件信息失败");
        close(fd);
        return -1;
    }
    
    if (st.st_size == 0) {
        printf("文件 \"%s\" 为空\n", filename);
        close(fd);
        return 0;
    }
    
    uint8_t *buffer = malloc(st.st_size);
    if (!buffer) {
        fprintf(stderr, "内存分配失败\n");
        close(fd);
        return -1;
    }
    
    ssize_t bytes_read = read(fd, buffer, st.st_size);
    if (bytes_read != st.st_size) {
        perror("读取文件失败");
        free(buffer);
        close(fd);
        return -1;
    }
    
    printf("文件: \"%s\" (大小: %ld 字节)\n", filename, st.st_size);
    printf("----------------------------------------\n");
    printf("Adler16:  0x%04X (%u)\n", adler16(buffer, st.st_size), adler16(buffer, st.st_size));
    printf("Adler32:  0x%08X (%u)\n", adler32(buffer, st.st_size), adler32(buffer, st.st_size));
    printf("Adler64:  0x%016lX (%lu)\n", adler64(buffer, st.st_size), adler64(buffer, st.st_size));
    printf("\n");
    
    free(buffer);
    close(fd);
    return 0;
}

/**
 * 性能测试
 */
void performance_test() {
    const size_t test_size = 1024 * 1024; // 1MB
    uint8_t *test_data = malloc(test_size);
    
    if (!test_data) {
        fprintf(stderr, "内存分配失败\n");
        return;
    }
    
    // 填充测试数据
    for (size_t i = 0; i < test_size; i++) {
        test_data[i] = (uint8_t)(i & 0xFF);
    }
    
    printf("性能测试 (数据大小: %zu 字节)\n", test_size);
    printf("----------------------------------------\n");
    
    clock_t start, end;
    
    // Adler16 性能测试
    start = clock();
    uint16_t result16 = adler16(test_data, test_size);
    end = clock();
    printf("Adler16: 0x%04X, 耗时: %.3f ms\n", result16, 
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    // Adler32 性能测试
    start = clock();
    uint32_t result32 = adler32(test_data, test_size);
    end = clock();
    printf("Adler32: 0x%08X, 耗时: %.3f ms\n", result32,
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    // Adler64 性能测试
    start = clock();
    uint64_t result64 = adler64(test_data, test_size);
    end = clock();
    printf("Adler64: 0x%016lX, 耗时: %.3f ms\n", result64,
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    free(test_data);
    printf("\n");
}

/**
 * 打印使用帮助
 */
void print_usage(const char *program_name) {
    printf("用法: %s [选项] [文件名/字符串]\n", program_name);
    printf("选项:\n");
    printf("  -h, --help     显示帮助信息\n");
    printf("  -f <文件>      计算文件的校验和\n");
    printf("  -s <字符串>    计算字符串的校验和\n");
    printf("  -t             运行性能测试\n");
    printf("  -d             运行演示测试\n");
    printf("\n");
    printf("示例:\n");
    printf("  %s -s \"Hello, World!\"    # 计算字符串校验和\n", program_name);
    printf("  %s -f /etc/passwd         # 计算文件校验和\n", program_name);
    printf("  %s -t                     # 运行性能测试\n", program_name);
    printf("  %s -d                     # 运行演示\n", program_name);
}

/**
 * 演示函数
 */
void run_demo() {
    printf("=== Adler 校验和算法演示 ===\n\n");
    
    // 测试空字符串
    calculate_string_checksums("");
    
    // 测试简单字符串
    calculate_string_checksums("a");
    calculate_string_checksums("abc");
    calculate_string_checksums("Hello, World!");
    
    // 测试长字符串
    calculate_string_checksums("The quick brown fox jumps over the lazy dog");
    
    // 测试重复字符
    calculate_string_checksums("aaaaaaaaaa");
    
    // 测试数字序列
    calculate_string_checksums("1234567890");
    
    printf("演示完成！\n\n");
}

int main(int argc, char *argv[]) {
    printf("=== Linux Adler 校验和计算器 ===\n\n");
    
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-s") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "错误: -s 选项需要字符串参数\n");
                return 1;
            }
            calculate_string_checksums(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "错误: -f 选项需要文件名参数\n");
                return 1;
            }
            if (calculate_file_checksums(argv[++i]) != 0) {
                return 1;
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            performance_test();
        } else if (strcmp(argv[i], "-d") == 0) {
            run_demo();
        } else {
            fprintf(stderr, "错误: 未知选项 '%s'\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }
    
    return 0;
} 