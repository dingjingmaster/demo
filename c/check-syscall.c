/*************************************************************************
> FileName: check-syscall.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月19日 星期五 10时00分22秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int main (int argc, char* argv[])
{
    if (-1 != syscall(SYS_getpid)) {
        printf ("getpid ok!\n");
    } else {
        printf ("getpid error!\n");
    }

#if 0
    if (-1 != syscall(SYS_aaa)) {
        printf ("aaa ok!\n");
    } else {
        printf ("aaa error!\n");
    }
#endif

    return 0;
}

