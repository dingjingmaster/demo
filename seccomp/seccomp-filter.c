/*************************************************************************
> FileName: seccomp-filter.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月13日 星期二 08时49分38秒
 ************************************************************************/
#include <unistd.h>
#include <seccomp.h>
#include <sys/syscall.h>


int main ()
{
    scmp_filter_ctx ctx;                // scmp 过滤上下文
    ctx = seccomp_init(SCMP_ACT_ALLOW); // 初始化过滤状态，允许所有系统调用
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);  // 添加需要限制的系统调用
    seccomp_load(ctx);                  // 装载上下文


    char* filename = "/bin/bash";

    char* argv[] = {filename, NULL};
    char* envp[] = {NULL};

    syscall(SYS_execve, filename, argv, envp);

    return 0;
}
