/*************************************************************************
> FileName: filesystem-type.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 28 Apr 2022 10:06:51 AM CST
 ************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/vfs.h>

// https://github.com/linux-test-project/ltp/tree/master/include/tst_fs.h
/* man 2 statfs or kernel-source/include/linux/magic.h */
#define TST_BTRFS_MAGIC    0x9123683E
#define TST_NFS_MAGIC      0x6969
#define TST_RAMFS_MAGIC    0x858458f6
#define TST_TMPFS_MAGIC    0x01021994
#define TST_V9FS_MAGIC     0x01021997
#define TST_XFS_MAGIC      0x58465342
#define TST_EXT2_OLD_MAGIC 0xEF51
/* ext2, ext3, ext4 have the same magic number */
#define TST_EXT234_MAGIC   0xEF53
#define TST_MINIX_MAGIC    0x137F
#define TST_MINIX_MAGIC2   0x138F
#define TST_MINIX2_MAGIC   0x2468
#define TST_MINIX2_MAGIC2  0x2478
#define TST_MINIX3_MAGIC   0x4D5A
#define TST_UDF_MAGIC      0x15013346
#define TST_SYSV2_MAGIC    0x012FF7B6
#define TST_SYSV4_MAGIC    0x012FF7B5
#define TST_UFS_MAGIC      0x00011954
#define TST_UFS2_MAGIC     0x19540119
#define TST_F2FS_MAGIC     0xF2F52010
#define TST_NILFS_MAGIC    0x3434
#define TST_EXOFS_MAGIC    0x5DF5
#define TST_OVERLAYFS_MAGIC 0x794c7630
#define TST_FUSE_MAGIC     0x65735546

// https://github.com/linux-test-project/ltp/tree/master/lib/tst_fs_type.c
const char *tst_fs_type_name(long f_type)
{
    switch (f_type) {
    case TST_TMPFS_MAGIC:
        return "tmpfs";
    case TST_NFS_MAGIC:
        return "nfs";
    case TST_V9FS_MAGIC:
        return "9p";
    case TST_RAMFS_MAGIC:
        return "ramfs";
    case TST_BTRFS_MAGIC:
        return "btrfs";
    case TST_XFS_MAGIC:
        return "xfs";
    case TST_EXT2_OLD_MAGIC:
        return "ext2";
    case TST_EXT234_MAGIC:
        return "ext2/ext3/ext4";
    case TST_MINIX_MAGIC:
    case TST_MINIX_MAGIC2:
    case TST_MINIX2_MAGIC:
    case TST_MINIX2_MAGIC2:
    case TST_MINIX3_MAGIC:
        return "minix";
    case TST_UDF_MAGIC:
        return "udf";
    case TST_SYSV2_MAGIC:
    case TST_SYSV4_MAGIC:
        return "sysv";
    case TST_UFS_MAGIC:
    case TST_UFS2_MAGIC:
        return "ufs";
    case TST_F2FS_MAGIC:
        return "f2fs";
    case TST_NILFS_MAGIC:
        return "nilfs";
    case TST_EXOFS_MAGIC:
        return "exofs";
    case TST_OVERLAYFS_MAGIC:
        return "overlayfs";
    case TST_FUSE_MAGIC:
        return "fuse";
    default:
        return "unknown";
    }
}

void print_filesystem(const char* path)
{
    if (path == NULL)
        return;

    struct statfs s;

    if (statfs(path, &s)) {
        fprintf(stderr, "statfs(%s) failed: %s\n", path, strerror(errno));
        return;
    }

    printf("'%s' filesystem: %s\n", path, tst_fs_type_name(s.f_type));
}

int main (int argc, char *argv[]) 
{
    if (argc <= 1) {
        printf ("please input device name\n");
    }

    print_filesystem("/");
    print_filesystem("/tmp");
    print_filesystem("/dev/sda");

    return 0;
}
