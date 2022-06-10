/*************************************************************************
> FileName: write.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月27日 星期日 15时11分05秒
 ************************************************************************/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("请输入要写入的文件路径!!\n");
        return -1;
    }

    char path[1024] = {0};
    strncpy(path, argv[1], strlen(argv[1]));
    strcat(path, "/aaa.txt");

    printf ("open file:%s\n", path);
    int fd = open (path, O_RDWR | O_CREAT);
    if (fd < 0) {
        printf ("打开文件描述符错误!\n");
        return -1;
    }

    posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);

    int i = 10;
    while (--i) {
        write (fd, "1111\n", 5);
        syncfs (fd);
    }
    close(fd);

    return 0;
}
