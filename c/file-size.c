/*************************************************************************
> FileName: file-size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月24日 星期一 09时44分58秒
 ************************************************************************/
#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/stat.h>


void lseek_size (const char* filePath)
{
    int size = 0;
    int fd = open (filePath, O_RDONLY);
    if (fd >= 0) {
        size = lseek(fd, 0, SEEK_END);
        close(fd);
    }

    printf("file size(lseek): %lu\n", size);
}

void stat_size (const char* filePath)
{
    int size = 0;
    struct stat st;

    if (0 == stat(filePath, &st)) {
        printf ("file size(stat): %lu\n", st.st_size);
    }

    if (0 == lstat(filePath, &st)) {
        printf ("file size(lstat): %lu\n", st.st_size);
    }

    int fd = open (filePath, O_RDONLY);
    if (fd >= 0) {
        if (0 == fstat(fd, &st)) {
            printf ("file size(fstat): %lu\n", st.st_size);
        }
        close(fd);
    }
}

void copy_file_size (const char* filePath)
{
    char newPath[PATH_MAX] = {0};

    snprintf(newPath, sizeof(newPath) - 1, "%s.bak", filePath);

    int fdIn = open (filePath, O_RDONLY);
    int fdOut = open (newPath, O_RDWR | O_CREAT, 0666);

    if (fdIn >=0 && fdOut >= 0) {
        int size = copy_file_range(fdIn, NULL, fdOut, NULL, 9999999, 0);
        printf("file size(copy_file_range): %d\n", size);
    }

    if (fdIn >= 0) {
        close(fdIn);
    }

    if (fdOut >= 0) {
        close(fdOut);
    }

    remove(newPath);
}


int main (int argc, char* argv[])
{
    if (2 != argc) {
        printf ("%s <file>\n", argv[0]);
        return 1;
    }

    lseek_size(argv[1]);
    stat_size(argv[1]);
    copy_file_size(argv[1]);

    return 0;
}
