/*************************************************************************
> FileName: max-fd.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月09日 星期三 11时07分14秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main (int argc, char* argv[])
{
	// 方法1: sysconf
    long open_max = sysconf(_SC_OPEN_MAX);
    printf("sysconf(_SC_OPEN_MAX): %ld\n", open_max);

    // 方法2: getrlimit
    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    printf("getrlimit: soft=%ld, hard=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    return 0;
}
