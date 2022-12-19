#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
    DIR* dir = NULL;
    int dirFd = 0;
    int fd = 0;
    int n = 0;

    dir = opendir(argv[1]);
    if (NULL == dir) {
        perror ("open dir error");
        return -1;
    }

    dirFd = dirfd (dir);
    if (-1 == dirFd) {
        perror ("dirfd error");
        return -1;
    }

    fd = openat (dirFd, "test.log", O_CREAT | O_RDWR | O_TRUNC, 0777);
    if (-1 == fd) {
        perror ("openat error");
        return -1;
    }

    n = write (fd, "hello world!\n", 12);

    close (fd);
    closedir(dir);

    return 0;
}
