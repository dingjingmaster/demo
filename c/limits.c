/*************************************************************************
> FileName: limits.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月20日 星期六 13时39分22秒
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <sys/resource.h>

void get_fd_limit()
{
    struct rlimit rl;
    if (0 == getrlimit(RLIMIT_NOFILE, &rl)) {
        printf ("Soft limit: %ld\n", rl.rlim_cur);
        printf ("Hard limit: %ld\n", rl.rlim_max);
    }
}

void set_fd_limit()
{
    struct rlimit rl;
    if (0 == getrlimit(RLIMIT_NOFILE, &rl)) {
        rl.rlim_cur = rl.rlim_max;
        setrlimit(RLIMIT_NOFILE, &rl);
    }
}

void set_fd_limit2()
{
    struct rlimit rl;
    if (0 == getrlimit(RLIMIT_NOFILE, &rl)) {
        rl.rlim_cur = INT32_MAX;
        rl.rlim_max = INT32_MAX;
        setrlimit(RLIMIT_NOFILE, &rl);
    }
}

int main ()
{
    get_fd_limit();
    set_fd_limit();
    get_fd_limit();
    set_fd_limit2();
    get_fd_limit();

    return 0;
}
