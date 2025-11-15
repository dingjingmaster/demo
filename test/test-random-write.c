/*************************************************************************
> FileName: test-random-write.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月15日 星期六 10时36分05秒
 ************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要写入的文件路径!!\n");
        return -1;
    }

    char buffer[101] = {0};
    const char* path = argv[1];

    printf ("open file:%s\n", path);
    int fd = open (path, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        printf ("打开文件描述符错误!\n");
        return -1;
    }

    // gen buffer
    for (int i = 0; i < sizeof(buffer) - 1; i++) {
        buffer[i] = '1';
    }

    // 写完
    for (int i = 0; i < (1 << 10); ++i) {
        write(fd, buffer, strlen(buffer));
        write(fd, "\n", 1);
    }

    // 随机写
    struct stat st;
    lstat(path, &st);
    off_t size = st.st_size;

    for (int j = 0; j < (1 << 20); ++j) {
        long pos = random() % size;
        lseek(fd, pos, SEEK_SET);
        write(fd, "2\n", 2);
    }

    close(fd);

    return 0;
}

