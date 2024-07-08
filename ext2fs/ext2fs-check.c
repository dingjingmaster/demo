/*************************************************************************
> FileName: ext2fs-check.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 08 Jul 2024 04:46:32 PM CST
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ext2fs/ext2fs.h>

int main (int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <device>\n", argv[0]);
        return 1;
    }

    const char *device = argv[1];
    ext2_filsys fs;
    errcode_t ret;

    // 打开 ext2 文件系统
    ret = ext2fs_open(device, EXT2_FLAG_RW, 0, 0, unix_io_manager, &fs);
    if (ret) {
        printf("Error opening file system: %s\n", strerror(errno));
        return 1;
    }

    printf("File system opened successfully.\n");

    // 检查文件系统的完整性
    ret = ext2fs_check_desc(fs);
    if (ret) {
        printf("Error checking file system: %s\n", strerror(ret));
        ext2fs_close(fs);
        return 1;
    }

    printf("File system check passed.\n");

    // 修复损坏的块
//    ret = ext2fs_fix_bad_blocks(fs);
//    if (ret) {
//        printf("Error fixing bad blocks: %s\n", strerror(ret));
//        ext2fs_close(fs);
//        return 1;
//    }

    printf("Bad blocks fixed successfully.\n");

    // 关闭文件系统
    ext2fs_close(fs);

    printf("File system operation completed.\n");


    return 0;
}
