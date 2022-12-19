/*************************************************************************
> FileName: openat1.txt
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 19 Dec 2022 09:53:54 AM CST
 ************************************************************************/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

int main (int argc, char* argv[])
{
    //int fd = openat (AT_FDCWD, "test.log", O_WRONLY | O_RDONLY | O_CREAT /*| O_EXCL*/, 0775);
    int fr = openat (AT_FDCWD, "test.log", O_RDWR | O_CREAT /*| O_EXCL*/, 0775);
    if (-1 == fr) {
        perror ("openat error");
        return -1;
    }

    write (fr, "hello world!\n", 14);
    fsync (fr);
    lseek (fr, 0, SEEK_SET);

    // 写
    int fw = openat (AT_FDCWD, "test-copy.log", O_RDWR | O_CREAT /*| O_EXCL*/, 0775);
    if (-1 == fw) {
        perror ("openat error");
        return -1;
    }

    //ioctl (fw, BTRFS_IOC_CLONE, fr);

    char buf[1024] = {0};

    int ret = read (fr, buf, sizeof(buf) - 1);
    if (ret < 0) {
        printf ("%d -- %d -- %d -- %s\n", fr, EBADF, errno, strerror(errno));
        return -1;
    }


    printf ("\nread: %s\n", buf);

    close (fr);

    return 0;
}
