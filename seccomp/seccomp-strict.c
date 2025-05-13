/*************************************************************************
> FileName: seccomp-strict.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月13日 星期二 08时34分50秒
 ************************************************************************/
#include <stdio.h>
#include <sys/prctl.h>

#include <sys/socket.h>
#include <linux/seccomp.h>


/**
 * @brief
 * 在最初引入的时候只支持了 strict mode，
 * 意味着只有 read ，write ，_exit ，_sigreturn 四个 system call 会被允许。
 * 一旦出现其他的 system call，进程会被立刻终止 (SIGKILL)
 */
int main(int argc, char* argv[]) 
{
    printf("Install seccomp\n");
    prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT);

    printf("Creating socket\n");
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    return 0;
}
