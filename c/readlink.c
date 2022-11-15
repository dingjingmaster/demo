/*************************************************************************
> FileName: readlink.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 15 Nov 2022 10:14:11 AM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入要查询的软连接，比如: /proc/<pid>/fd/1\n");
        return -1;
    }

    char buf[1024] = {0};
    ssize_t size = readlink (argv[1], buf, sizeof(buf) - 1);

    printf ("%ld -- %s\n", size, buf);

    return 0;
}
