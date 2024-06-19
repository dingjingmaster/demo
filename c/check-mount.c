/*************************************************************************
> FileName: check-mount.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 19 Jun 2024 04:08:27 PM CST
 ************************************************************************/
#include <stdio.h>
#include <sys/vfs.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf ("请输入要检查的文件夹[检查此文件夹是否是挂载点]\n");
        return (-1);
    }
    struct statfs s;

    int ret = statfs (argv[1], &s);
    if (0 == ret) {
        printf ("%s mounted!\n", argv[1]);
    }
    else {
        printf ("%s unmounted!\n", argv[1]);
    }

    return 0;
}
