/*************************************************************************
> FileName: linux-version.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年03月19日 星期五 10时49分28秒
 ************************************************************************/
#include <stdio.h>
#include <linux/version.h>

int main (int argc, char* argv[])
{
    printf ("version: %d\n", LINUX_VERSION_CODE);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
    printf ("linux kernel >= 5.4.0\n");
#else
    printf ("linux kernel < 5.4.0\n");
#endif

    return 0;
}
