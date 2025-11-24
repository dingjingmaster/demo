/*************************************************************************
> FileName: lseek.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月22日 星期六 14时56分11秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main (int argc, char* argv[])
{
    if (2 != argc) {
        printf ("%s <file>\n", argv[0]);
        return 1;
    }

    int size = 0;
    int fd = open (argv[1], O_RDONLY, 0);
    if (fd >= 0) {
        size = lseek(fd, 0, SEEK_END);
        close(fd);
    }

    printf("file size: %lu\n", size);

    return 0;
}
