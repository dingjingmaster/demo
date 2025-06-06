/*
 * simplefs.c - 一个简单的 Linux 内核文件系统模块
 *
 * 本模块实现了一个最基础的文件系统框架，支持挂载、卸载，并显示一个空目录。
 * 它展示了 Linux 内核文件系统开发的基本步骤和关键概念。
 *
 * 文件系统开发的关键概念：
 * 1. 文件系统类型 (struct file_system_type)：定义文件系统的名称、挂载方法等。
 * 2. 超级块 (struct super_block)：文件系统的元数据，包含文件系统的全局信息。
 * 3. inode：文件或目录的元数据，包含权限、大小、时间戳等。
 * 4. dentry：目录项，用于缓存文件路径到 inode 的映射。
 * 5. 文件操作 (struct file_operations)：定义文件读写、目录遍历等操作。
 * 6. inode 操作 (struct inode_operations)：定义 inode 的查找、创建、删除等操作。
 *
 * 本模块仅实现了一个空目录，展示了如何：
 * - 注册文件系统
 * - 挂载文件系统
 * - 创建根 inode
 * - 实现目录的 lookup 和 iterate 操作
 */

#include <linux/module.h>      // 内核模块支持
#include <linux/fs.h>          // 文件系统相关定义
#include <linux/init.h>        // 模块初始化和清理
#include <linux/slab.h>        // 内存分配
#include <linux/string.h>      // 字符串操作
#include <linux/statfs.h>      // 文件系统统计信息
#include <linux/buffer_head.h> // 缓冲区头
#include <linux/errno.h>       // 错误码
#include <linux/types.h>       // 基本数据类型
#include <linux/version.h>     // 内核版本
#include <linux/blkdev.h>      // 块设备支持
#include <linux/user_namespace.h> // 用户命名空间

// 文件系统魔数，用于标识文件系统类型
#define SIMPLEFS_MAGIC 0x12345678
// 文件系统块大小，通常为 4KB
#define SIMPLEFS_BLOCK_SIZE 4096
// 文件系统名称
#define SIMPLEFS_NAME "simplefs"

// 前向声明：文件系统挂载函数
static struct dentry *simplefs_mount(struct file_system_type *fs_type,
                                   int flags, const char *dev_name,
                                   void *data);
// 前向声明：填充超级块函数
static int simplefs_fill_super(struct super_block *sb, void *data, int silent);
// 前向声明：文件系统统计信息函数
static int simplefs_statfs(struct dentry *dentry, struct kstatfs *buf);
// 前向声明：目录查找函数
static struct dentry *simplefs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags);
// 前向声明：目录遍历函数
static int simplefs_iterate(struct file *file, struct dir_context *ctx);

// 定义文件系统类型
static struct file_system_type simplefs_type = {
    .owner = THIS_MODULE,      // 模块所有者
    .name = SIMPLEFS_NAME,     // 文件系统名称
    .mount = simplefs_mount,   // 挂载函数
    .kill_sb = kill_block_super, // 卸载函数
    .fs_flags = FS_REQUIRES_DEV, // 需要块设备
};

// 定义超级块操作
static const struct super_operations simplefs_super_ops = {
    .statfs = simplefs_statfs, // 文件系统统计信息
    .drop_inode = generic_delete_inode, // 删除 inode
};

// 定义目录 inode 操作
static const struct inode_operations simplefs_dir_inode_operations = {
    .lookup = simplefs_lookup, // 目录查找
};

// 定义目录文件操作
static const struct file_operations simplefs_dir_operations = {
    .owner = THIS_MODULE,      // 模块所有者
    .iterate_shared = simplefs_iterate, // 目录遍历
};

// 文件系统挂载函数
static struct dentry *simplefs_mount(struct file_system_type *fs_type,
                                   int flags, const char *dev_name,
                                   void *data)
{
    // 调用 mount_bdev 挂载块设备
    return mount_bdev(fs_type, flags, dev_name, data, simplefs_fill_super);
}

// 填充超级块函数
static int simplefs_fill_super(struct super_block *sb, void *data, int silent)
{
    struct inode *root_inode;
    int ret;

    // 设置超级块魔数，用于标识文件系统
    sb->s_magic = SIMPLEFS_MAGIC;
    // 设置超级块操作
    sb->s_op = &simplefs_super_ops;
    // 设置最大文件大小
    sb->s_maxbytes = MAX_LFS_FILESIZE;
    // 设置块大小
    sb->s_blocksize = SIMPLEFS_BLOCK_SIZE;
    // 设置块大小的位数
    sb->s_blocksize_bits = blksize_bits(SIMPLEFS_BLOCK_SIZE);

    // 创建根 inode
    root_inode = new_inode(sb);
    if (!root_inode)
        return -ENOMEM;

    // 设置根 inode 的属性
    root_inode->i_ino = 1; // inode 号
    root_inode->i_mode = S_IFDIR | 0755; // 目录权限
    inode_set_atime_to_ts(root_inode, current_time(root_inode)); // 访问时间
    inode_set_mtime_to_ts(root_inode, current_time(root_inode)); // 修改时间
    inode_set_ctime_to_ts(root_inode, current_time(root_inode)); // 创建时间
    root_inode->i_uid = GLOBAL_ROOT_UID; // 用户 ID
    root_inode->i_gid = GLOBAL_ROOT_GID; // 组 ID

    // 设置根 inode 的操作
    root_inode->i_op = &simplefs_dir_inode_operations; // inode 操作
    root_inode->i_fop = &simplefs_dir_operations; // 文件操作

    // 创建根目录项
    sb->s_root = d_make_root(root_inode);
    if (!sb->s_root) {
        iput(root_inode);
        return -ENOMEM;
    }

    return 0;
}

// 文件系统统计信息函数
static int simplefs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
    buf->f_type = SIMPLEFS_MAGIC; // 文件系统类型
    buf->f_bsize = SIMPLEFS_BLOCK_SIZE; // 块大小
    buf->f_blocks = 0; // 总块数
    buf->f_bfree = 0; // 空闲块数
    buf->f_bavail = 0; // 可用块数
    buf->f_files = 0; // 总文件数
    buf->f_ffree = 0; // 空闲文件数
    buf->f_namelen = NAME_MAX; // 最大文件名长度
    return 0;
}

// 模块初始化函数
static int __init init_simplefs(void)
{
    // 注册文件系统
    int ret = register_filesystem(&simplefs_type);
    if (ret)
        printk(KERN_ERR "Failed to register simplefs\n");
    else
        printk(KERN_INFO "SimpleFS registered successfully\n");
    return ret;
}

// 模块清理函数
static void __exit exit_simplefs(void)
{
    // 注销文件系统
    int ret = unregister_filesystem(&simplefs_type);
    if (ret)
        printk(KERN_ERR "Failed to unregister simplefs\n");
    else
        printk(KERN_INFO "SimpleFS unregistered successfully\n");
}

// 模块初始化和清理
module_init(init_simplefs);
module_exit(exit_simplefs);

// 模块许可证和作者信息
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple filesystem for Linux");

// 目录查找函数实现
static struct dentry *simplefs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags) {
    // 空目录，直接返回 NULL
    d_add(dentry, NULL);
    return NULL;
}

// 目录遍历函数实现
static int simplefs_iterate(struct file *file, struct dir_context *ctx) {
    // 只显示 . 和 ..
    if (ctx->pos)
        return 0;
    dir_emit_dots(file, ctx);
    ctx->pos = 2;
    return 0;
}
