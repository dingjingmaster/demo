/*************************************************************************
> FileName: create-time.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年09月02日 星期三 11时32分57秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>

int main (int argc, char* argv[])
{
    if (argc <= 1) {
        printf ("Usage:\ncommand filename\n");
        return -1;
    }

    int fd = 0;
    int ret = 0;
    struct statx buf;
    int i = 0;
    char* tmp = NULL;
    char* name = NULL;
    char path[2048] = {0};

    strncpy (path, argv[1], sizeof path);

    for (i = 0, tmp = path; (tmp != NULL) && (path[i] != '\0'); tmp = strstr(path + i, "/"), i = tmp - path + 1){
        if (NULL != tmp)
            name = tmp;
    }

    if (name != path) {
        *name = '\0';
        name = name + 1;
    }

    fd = open (path, O_RDONLY);
    if (fd < 0) {
        printf ("open file error!");
        return -1;
    }

    ret = statx (fd, name, AT_SYMLINK_NOFOLLOW, STATX_BASIC_STATS, &buf);
    if (0 == ret) {
        printf ("create time: %d\n", buf.stx_ctime.tv_sec);
        printf ("access time: %d\n", buf.stx_atime.tv_sec);
        printf ("modify time: %d\n", buf.stx_mtime.tv_sec);
    } else if (EACCES == errno) {
        printf ("permission is denied\n");
    } else if (EBADF == errno) {
        printf ("dirfd is not a valid opdfn file descriptor\n");
    } else if (EFAULT == errno) {
        printf ("pathname or statxbuf is NULL or points is invalid\n");
    } else if (EINVAL== errno) {
        printf ("invalid flag\n");
    } else if (ELOOP == errno) {
        printf ("eloop\n");
    } else if (ENAMETOOLONG == errno) {
        printf ("path name is too long\n");
    } else if (ENOENT == errno) {
        printf ("path name does not exist\n");
    } else if (ENOMEM == errno) {
        printf ("out of memory\n");
    } else if (ENOTDIR == errno) {
        printf ("path not a directory\n");
    } else {
        printf ("error\n");
    }

    return 0;
}
