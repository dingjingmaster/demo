/*************************************************************************
> FileName: statfs.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年03月12日 星期四 17时38分47秒
 ************************************************************************/
#include <stdio.h>
#include <sys/statfs.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("输入文件夹\n");
        return -1;
    }

    struct statfs fsinfo;
    if (0 != statfs(argv[1], &fsinfo)) {
        printf("statfs error!\n");
        return -1;
    }
    printf ("Filesystem Type: 0x%x\n", fsinfo.f_type);

    if (0x49414e44 == fsinfo.f_type) {
        printf("safe_fs\n");
    }

    return 0;
}
