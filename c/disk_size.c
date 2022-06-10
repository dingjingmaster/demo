/*************************************************************************
> FileName: disk_size.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年11月12日 星期四 20时16分42秒
 ************************************************************************/
#include <stdio.h>
#include <sys/statfs.h>

int main()
{
    struct statfs diskInfo;

    statfs("/", &diskInfo);
    unsigned long long blocksize = diskInfo.f_bsize;
    unsigned long long totalsize = blocksize * diskInfo.f_blocks;
    printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n", totalsize, totalsize>>10, totalsize>>20, totalsize>>30);

    unsigned long long freeDisk = diskInfo.f_bfree * blocksize;
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize;
    printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n", freeDisk>>20, freeDisk>>30, availableDisk>>20, availableDisk>>30);

    return 0;
}
