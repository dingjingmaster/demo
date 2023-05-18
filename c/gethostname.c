/*************************************************************************
> FileName: gethostname.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 10 May 2023 08:48:52 AM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    char buf[256] = {0};

    gethostname(buf, sizeof(buf) - 1);

    printf ("%s\n", buf);

    return 0;
}
