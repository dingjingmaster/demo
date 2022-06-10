/*************************************************************************
> FileName: file-open.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月26日 星期六 19时14分31秒
 ************************************************************************/

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>


int main (int argc, char* argv[])
{
    if (argc < 2) {
        printf ("input file\n");
        return -1;
    }

    char buf[32] = {0};
    int fd = open (argv[1], O_CREAT | O_RDWR);
    if (fd < 0) {
        printf ("error: %d, %s\n", errno, strerror(errno));
        return -1;
    } else {
        while (1) {
            memset (buf, 0, sizeof(buf));
            snprintf (buf, sizeof(buf) - 1, "%d\n", getpid());
            printf ("%s\n", buf);
            write (fd, buf, strlen(buf));
            sleep (2);
        }
    }


    return 0;
}
