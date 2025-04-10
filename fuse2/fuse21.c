#define FUSE_USE_VERSION 21
#include <fuse.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>


static void signal_handle_sigsegv(int signal);

static int fuse_getattr(const char* path, struct stat *statbuf);
static int fuse_readlink(const char* path, char *buf, size_t size);
static int fuse_getdir(const char* path, fuse_dirh_t, fuse_dirfil_t_compat entry);
static int fuse_mknod(const char* path, mode_t mode, dev_t dev);
static int fuse_mkdir(const char* path, mode_t mode);
static int fuse_unlink(const char* path);
static int fuse_rmdir(const char* path);
static int fuse_symlink(const char* path, const char* link);
static int fuse_rename(const char *path, const char *link);
static int fuse_link(const char* path, const char* link);
static int fuse_chmod(const char* path, mode_t mode);
static int fuse_chown(const char* path, uid_t uid, gid_t gid);
static int fuse_truncate(const char* path, off_t length);
static int fuse_utime(const char *path, struct utimbuf* info);
static int fuse_open(const char* path, int file);
static int fuse_read(const char* path, char*, size_t size, off_t);
static int fuse_write(const char* path, const char* buf, size_t size, off_t);
static int fuse_statfs(const char* path, struct statfs* buf);
static int fuse_flush(const char* path);
static int fuse_release(const char* path, int);
static int fuse_fsync(const char* path, int fd);
static int fuse_setxattr(const char* path, const char*, const char*, size_t, int);
static int fuse_getxattr(const char* path, const char*, char*, size_t);
static int fuse_listxattr(const char* path, char*, size_t);
static int fuse_removexattr(const char* path, const char*);


static struct fuse_operations ops = {
    .getattr = fuse_getattr,
    .readlink = fuse_readlink,
    .getdir = fuse_getdir,
    .mknod = fuse_mknod,
    .mkdir = fuse_mkdir,
    .unlink = fuse_unlink,
    .rmdir = fuse_rmdir,
    .symlink = fuse_symlink,
    .rename = fuse_rename,
    .link = fuse_link,
    .chmod = fuse_chmod,
    .chown = fuse_chown,
    .truncate = fuse_truncate,
    .utime = fuse_utime,
    .open = fuse_open,
    .read = fuse_read,
    .write = fuse_write,
    .statfs = fuse_statfs,
    .flush = fuse_flush,
    .release = fuse_release,
    .fsync = fuse_fsync,
    .setxattr = fuse_setxattr,
    .getxattr = fuse_getxattr,
    .listxattr = fuse_listxattr,
    .removexattr = fuse_removexattr,
};


int main(int argc, char* argv[])
{
    int res = 0;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    if (signal(SIGSEGV, signal_handle_sigsegv) == SIG_ERR) {
        syslog(LOG_ERR, "Failed to initialize signals");
        return EXIT_FAILURE;
    }

    res = fuse_main(args.argc, args.argv, &ops);

    fuse_opt_free_args(&args);

    return res;
}

static void signal_handle_sigsegv(int signal)
{
    size_t i = 0;
    size_t size = 0;
    char **strings = NULL;
    void *array[10] = {NULL};

    syslog(LOG_INFO,"========================================");
    syslog(LOG_INFO,"Segmentation Fault.  Starting backtrace:");
    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    for (i = 0; i < size; i++) {
        syslog(LOG_INFO,"%s\n", strings[i]);
    }
    syslog(LOG_INFO,"========================================");

    abort();
    (void) signal;
}

static int fuse_getattr(const char* path, struct stat *statbuf)
{
    syslog(LOG_INFO,"getattr path: %s", path);
    if (!path || !statbuf) {
        errno = EINVAL;
        syslog(LOG_INFO,"getattr, invalid params");
        return -EINVAL;
    }

    if (0 == strcmp(path, "/")) {
        statbuf->st_dev = 0;
        statbuf->st_ino = 0;
        statbuf->st_nlink = 1;
        statbuf->st_mode = S_IFDIR | 0755;  // 文件类型 + 权限
        statbuf->st_uid = 0;
        statbuf->st_gid = 0;
        statbuf->st_size = 4096;        // 大小属性，可以随意写
        statbuf->st_blksize = 4096;
        statbuf->st_blocks = 1;
        statbuf->st_atime = 0;
        statbuf->st_mtime = 0;
        statbuf->st_ctime = 0;
    }
    else if (0 == strcmp(path, "/.Trash")) {
        statbuf->st_dev = 0;
        statbuf->st_ino = 0;
        statbuf->st_nlink = 1;
        statbuf->st_mode = S_IFDIR | 0755;  // 文件类型 + 权限
        statbuf->st_uid = 0;
        statbuf->st_gid = 0;
        statbuf->st_size = 4096;        // 大小属性，可以随意写
        statbuf->st_blksize = 4096;
        statbuf->st_blocks = 1;
        statbuf->st_atime = 0;
        statbuf->st_mtime = 0;
        statbuf->st_ctime = 0;
    }
    else if (0 == strcmp(path, "/.Trash-1000")) {
        statbuf->st_dev = 0;
        statbuf->st_ino = 0;
        statbuf->st_nlink = 1;
        statbuf->st_mode = S_IFDIR | 0755;  // 文件类型 + 权限
        statbuf->st_uid = 1000;
        statbuf->st_gid = 0;
        statbuf->st_size = 4096;        // 大小属性，可以随意写
        statbuf->st_blksize = 4096;
        statbuf->st_blocks = 1;
        statbuf->st_atime = 0;
        statbuf->st_mtime = 0;
        statbuf->st_ctime = 0;
    }
    else if (0 == strcmp(path, "/dir1")) {
        statbuf->st_dev = 0;
        statbuf->st_ino = 0;
        statbuf->st_nlink = 1;
        statbuf->st_mode = S_IFDIR | 0755;  // 文件类型 + 权限
        statbuf->st_uid = 0;
        statbuf->st_gid = 0;
        statbuf->st_size = 8192;        // 大小属性，可以随意写
        statbuf->st_blksize = 4096;
        statbuf->st_blocks = 2;
        statbuf->st_atime = 0;
        statbuf->st_mtime = 0;
        statbuf->st_ctime = 0;
    }
    else if (0 == strcmp(path, "/file1")) {
        statbuf->st_dev = 0;
        statbuf->st_ino = 0;
        statbuf->st_nlink = 1;
        statbuf->st_mode = S_IFREG | 0755;  // 文件类型 + 权限
        statbuf->st_uid = 1000;
        statbuf->st_gid = 0;
        statbuf->st_size = 8192000;        // 大小属性，可以随意写
        statbuf->st_blksize = 4096;
        statbuf->st_blocks = 2000;
        statbuf->st_atime = 0;
        statbuf->st_mtime = 0;
        statbuf->st_ctime = 0;
    }
    else {
        return -ENOENT;
    }

    return 0;
}

static int fuse_readlink(const char* path, char *buf, size_t size)
{
    syslog(LOG_INFO,"readlink");
    return 0;
}

static int fuse_getdir(const char* path, fuse_dirh_t dirh, fuse_dirfil_t_compat entry)
{
    syslog(LOG_INFO,"getdir, path: %s", path);

    if (!path || (!entry && !dirh)) {
        errno = EINVAL;
        syslog(LOG_ERR,"getdir: invalid params");
        return -EINVAL;
    }

    if (0 == strcmp(path, "/")) {
        entry(dirh, ".", S_IFDIR);
        entry(dirh, "..", S_IFDIR);
        entry(dirh, ".Trash", S_IFDIR);
        entry(dirh, ".Trash-1000", S_IFDIR);
        entry(dirh, "dir1", S_IFDIR);
        entry(dirh, "file1", S_IFREG);
    }
    else if (0 == strcmp(path, "/dir1")) {
        entry(dirh, ".", S_IFDIR);
        entry(dirh, "..", S_IFDIR);
    }
    else {
        entry(dirh, ".", S_IFDIR);
        entry(dirh, "..", S_IFDIR);
    }

    return 0;
}

static int fuse_mknod(const char* path, mode_t mode, dev_t dev)
{
    syslog(LOG_INFO,"mknod, path: %s", path);
    return 0;
}

static int fuse_mkdir(const char* path, mode_t mode)
{
    syslog(LOG_INFO,"mkdir");
    return 0;
}

static int fuse_unlink(const char* path)
{
    syslog(LOG_INFO,"unlink");
    return 0;
}

static int fuse_rmdir(const char* path)
{
    syslog(LOG_INFO,"rmdir");
    return 0;
}

static int fuse_symlink(const char* path, const char* link)
{
    syslog(LOG_INFO,"symlink");
    return 0;
}

static int fuse_rename(const char *path, const char *link)
{
    syslog(LOG_INFO,"rename");
    return 0;
}

static int fuse_link(const char* path, const char* link)
{
    syslog(LOG_INFO,"link");
    return 0;
}

static int fuse_chmod(const char* path, mode_t mode)
{
    syslog(LOG_INFO,"chmod");
    return 0;
}

static int fuse_chown(const char* path, uid_t uid, gid_t gid)
{
    syslog(LOG_INFO,"chown");
    return 0;
}

static int fuse_truncate(const char* path, off_t length)
{
    syslog(LOG_INFO,"truncate");
    return 0;
}

static int fuse_utime(const char *path, struct utimbuf* info)
{
    syslog(LOG_INFO,"utime");
    return 0;
}

static int fuse_open(const char* path, int file)
{
    syslog(LOG_INFO,"open, path: %s", path);

    if (!path) {
        syslog(LOG_ERR, "open failed!");
        return -EINVAL;
    }

    if (0 == strcmp(path, "/")) {
        return -EBADE;
    }
    if (0 == strcmp(path, "/dir1")) {
        return -EBADE;
    }
    if (0 == strcmp(path, "/file1")) {
        return 0;
    }

    return -ENOENT;
}

static int fuse_read(const char* path, char* buf, size_t size, off_t offset)
{
    syslog(LOG_INFO,"read, path: %s, size: %ld, offset: %ld", path, size, offset);

    if (0 == strcmp(path, "/file1")) {
        if (size > 0 && buf) {
            for (int i = 0; i < size - 1; i++) {
                buf[i] = 'A';
            }
            buf[size - 1] = '\0';
        }
    }

    return size;
}

static int fuse_write(const char* path, const char* buf, size_t size, off_t)
{
    syslog(LOG_INFO,"write, path: %s", path);

    if (0 == strcmp(path, "/file1")) {
        syslog(LOG_ERR, "write: %s", buf);
        return size;
    }

    return -EACCES;
}

static int fuse_statfs(const char* path, struct statfs* buf)
{
    syslog(LOG_INFO,"statfs, path: %s", path);
    return 0;
}

static int fuse_flush(const char* path)
{
    syslog(LOG_INFO,"flush");
    return 0;
}

static int fuse_release(const char* path, int)
{
    syslog(LOG_INFO,"release");
    return 0;
}

static int fuse_fsync(const char* path, int fd)
{
    syslog(LOG_INFO,"fsync");
    return 0;
}

static int fuse_setxattr(const char* path, const char*, const char*, size_t, int)
{
    syslog(LOG_INFO,"setxattr");

    printf("Not implemented\n");

    return -EINVAL;
}

static int fuse_getxattr(const char* path, const char* k, char* v, size_t size)
{
    syslog(LOG_INFO,"getxattr");
    (void) path;

    if (0 == strcmp(k, "key1")) {
        const char* val = "value1";
        const int valLen = strlen(val);
        if (size == 0) {
            return valLen + 1;
        }

        if (size >= valLen) {
            memcpy(v, val, valLen);
            return valLen;
        }
    }
    else if (0 == strcmp(k, "key2")) {
        const char* val = "value2";
        const int valLen = strlen(val);
        if (size == 0) {
            return valLen;
        }

        if (size >= valLen) {
            memcpy(v, val, valLen);
            return valLen;
        }
    }
    else if (0 == strcmp(k, "key3")) {
        const char* val = "value3";
        const int valLen = strlen(val);
        if (size == 0) {
            return valLen;
        }

        if (size >= valLen) {
            memcpy(v, val, valLen);
            return valLen;
        }
    }

    return -ENODATA;
}

static int fuse_listxattr(const char* path, char* list, size_t size)
{
    syslog(LOG_INFO,"listxattr, path: %s", path);
    return 0;

    (void) path;
    static const char* xattrs = "key1=v1\0key2=v2\0key3=v3\0";
    static const int xattrsSize = 28; //sizeof(xattrs);

    if (0 == strcmp(path, "/file1")) {
        if (NULL == list || size == 0) {
            return xattrsSize;
        }
        if (size >= xattrsSize) {
            memcpy(list, xattrs, xattrsSize);
            return xattrsSize;
        }
    }

    syslog(LOG_ERR, "ENODATA");

    return -ENODATA;
}

static int fuse_removexattr(const char* path, const char*)
{
    syslog(LOG_INFO,"removexattr");
    return 0;
}

