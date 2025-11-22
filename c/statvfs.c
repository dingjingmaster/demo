/*************************************************************************
> FileName: statvfs.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月19日 星期三 10时36分08秒
 ************************************************************************/
#include <stdio.h>
#include <sys/statvfs.h>


int main (int argc, char* argv[])
{
    struct statvfs st;

    if (0 != statvfs(argv[1], &st)) {
        printf("statvfs error\n");
        return -1;
    }

    printf("is read only: %s\n", ((st.f_flag&ST_RDONLY) ? "true" : "false"));

    return 0;
}
