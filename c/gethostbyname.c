/*************************************************************************
> FileName: gethostbyname.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 10 May 2023 08:53:11 AM CST
 ************************************************************************/
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>


int main (int argc, char* argv[])
{
    char buf[256] = {0};

    gethostname(buf, sizeof(buf) - 1);

    struct hostent* hp = gethostbyname(buf);
    if (NULL == hp) {
        return -1;
    }

    printf("%s\n", buf);

    inet_ntop(hp->h_addrtype, hp->h_addr, buf, sizeof(buf) - 1);

    printf("%s\n", buf);

    return 0;
}
