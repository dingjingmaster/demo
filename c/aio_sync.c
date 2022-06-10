/*************************************************************************
> FileName: aio_sync.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年12月23日 星期三 20时36分52秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <aio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char* argv[])
{
    int ret;
    struct aiocb cb;
    char buf[256] = {0};

    memset(&cb, 0, sizeof(cb));
    cb.aio_fildes = 0;
    cb.aio_nbytes = 256;
    cb.aio_buf = buf;

    ret = aio_read(&cb);
    if (-1 == ret) {
        perror("aio_read");
        exit(1);
    }

    printf("Aio read task!\n");

    while (1) {
        ret = aio_error(&cb);
        if (ret == EINPROGRESS) {
            continue;
        } else {
            break;
        }
    }
    
    printf("read:%s, ret = %d\n", (char *)cb.aio_buf, aio_return(&cb));

    return 0;
}
