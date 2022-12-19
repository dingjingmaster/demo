/*************************************************************************
> FileName: openat1.txt
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 19 Dec 2022 09:53:54 AM CST
 ************************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
    int fd = 0;
    int n = 0;

    fd = openat (AT_FDCWD, "test.log", O_CREAT | O_RDWR | O_EXCL, 0777);
    if (-1 == fd) {
        perror ("openat error");
        return -1;
    }

    n = write (fd, "hello world!\n", 12);

    close (fd);

    return 0;
}
