/*************************************************************************
> FileName: write2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 02 Aug 2022 07:20:59 PM CST
 ************************************************************************/
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    int fd = open ("kk.txt", O_RDWR | O_CREAT, 0777);

    for (int i = 0; i < 9999000000; ++i) {
        write (fd, "bb\n", 4);
        syncfs (fd);
    }
}
