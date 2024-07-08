/*************************************************************************
> FileName: ext2fs-format.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 08 Jul 2024 05:10:03 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ext2fs/ext2fs.h>

int main(int argc, char *argv[]) 
{
    errcode_t retval;
    ext2_filsys fs;
    char *device;
    struct ext2_super_block super;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <device>\n", argv[0]);
        exit(1);
    }

    device = argv[1];

    // 初始化ext2fs错误表
    initialize_ext2_error_table();

    // 创建新的文件系统
    retval = ext2fs_initialize(device, 0, &super, unix_io_manager, &fs);
    if (retval) {
        printf ("%s %d %s\n", argv[0], retval, "while initializing filesystem");
        exit(1);
    }

    // 设置文件系统参数
    fs->super->s_feature_compat = EXT2_FEATURE_COMPAT_DIR_INDEX;
    fs->super->s_feature_incompat = EXT2_FEATURE_INCOMPAT_FILETYPE;
    fs->super->s_feature_ro_compat = EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER;

    // 分配inode和块位图
    retval = ext2fs_allocate_tables(fs);
    if (retval) {
        printf("%s %d %s\n", argv[0], retval, "while allocating tables");
        exit(1);
    }

    // 创建根目录
    retval = ext2fs_mkdir(fs, EXT2_ROOT_INO, EXT2_ROOT_INO, 0);
    if (retval) {
        printf("%s %d %s\n", argv[0], retval, "while creating root dir");
        exit(1);
    }

    // 写入超级块和块组描述符
    retval = ext2fs_flush(fs);
    if (retval) {
        printf("%s %d %s\n", argv[0], retval, "while writing out superblock");
        exit(1);
    }

    // 关闭文件系统
    ext2fs_close(fs);

    printf("Ext2 filesystem successfully created on %s\n", device);

    return 0;
}
