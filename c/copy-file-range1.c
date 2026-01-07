/*************************************************************************
> FileName: copy-file-range.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年01月07日 星期三 15时17分23秒
 ************************************************************************/
#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/**
 * 使用 copy_file_range 复制文件
 * 编译命令: gcc copy_example.c -o copy_example
 */
int main(int argc, char *argv[]) {
    int fd_in, fd_out;
    struct stat stat_in;
    ssize_t ret;

    if (argc != 3) {
        fprintf(stderr, "用法: %s <源文件> <目标文件>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 1. 打开源文件
    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("打开源文件失败");
        exit(EXIT_FAILURE);
    }

    // 获取源文件信息（主要是获取大小）
    if (fstat(fd_in, &stat_in) == -1) {
        perror("获取文件状态失败");
        close(fd_in);
        exit(EXIT_FAILURE);
    }

    // 2. 创建/打开目标文件
    fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, stat_in.st_mode);
    if (fd_out == -1) {
        perror("创建目标文件失败");
        close(fd_in);
        exit(EXIT_FAILURE);
    }

    // 3. 循环调用 copy_file_range 直至完成
    // 原因是单次调用可能无法拷贝完所有数据，或者受限于内核限制（如 2GB）
    size_t len = stat_in.st_size;
    posix_fadvise64(fd_in, 0, 0, POSIX_FADV_SEQUENTIAL);

    while (len > 0) {
        ret = copy_file_range(fd_in, NULL, fd_out, NULL, len, 0);
        if (ret == -1) {
            perror("copy_file_range 拷贝失败");
            close(fd_in);
            close(fd_out);
            exit(EXIT_FAILURE);
        }
        if (ret == 0) break; // 已到文件末尾

        len -= ret;
    }

    printf("文件复制成功！\n");

    close(fd_in);
    close(fd_out);
    return 0;
}

