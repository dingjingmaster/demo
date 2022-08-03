/*************************************************************************
> FileName: read.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 02 Aug 2022 07:26:46 PM CST
 ************************************************************************/
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    int fd = open ("kk.txt", O_RDONLY);

    char buf[102400] = {0};
    read (fd, buf, 102400 - 1);

    printf (buf);
}
