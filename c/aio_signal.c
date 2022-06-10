/*************************************************************************
> FileName: aio_signal.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年12月23日 星期三 20时18分24秒
 ************************************************************************/
#include <aio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/**
 * @brief int aio_read(struct aiocb *aiocbp); 是read的异步操作API 
 */

void async_process(int s, siginfo_t *info, void *context)
{
    struct aiocb *cbp = (struct aiocb *)(info->si_ptr);
    printf("read:%s\n", (char *)cbp->aio_buf);	
}

int main(int argc, char* argv[])
{
    struct aiocb cb;
    char buf[256] = {0};
    int ret;
    struct sigaction act;
    act.sa_sigaction = async_process;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO | SA_RESTART;

    sigaction(SIGUSR1, &act, NULL);

    memset(&cb, 0, sizeof(cb));

    cb.aio_fildes = 0;
    cb.aio_nbytes = 256;
    cb.aio_buf = buf;
    cb.aio_sigevent.sigev_value.sival_ptr = &cb;
    cb.aio_sigevent.sigev_signo = SIGUSR1;
    cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;

    ret = aio_read(&cb);
    if (-1 == ret) {
        perror("aio_read");
        exit(1);
    }	
	
    printf("Aio read task!\n");

    sleep(1000);

    return 0;
}
