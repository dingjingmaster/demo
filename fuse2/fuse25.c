//
// Created by dingjing on 25-4-10.
//
#define FUSE_USE_VERSION 25
#include <fuse.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>


static int fuse_getattr (const char *, struct stat *);
static int fuse_readlink (const char *, char *, size_t);
static int fuse_getdir (const char *, fuse_dirh_t, fuse_dirfil_t);
static int fuse_mknod (const char *, mode_t, dev_t);
static int fuse_mkdir (const char *, mode_t);
static int fuse_unlink (const char *);
static int fuse_rmdir (const char *);
static int fuse_symlink (const char *, const char *);
static int fuse_rename (const char *, const char *);
static int fuse_link (const char *, const char *);
static int fuse_chmod (const char *, mode_t);
static int fuse_chown (const char *, uid_t, gid_t);
static int fuse_truncate (const char *, off_t);
static int fuse_utime (const char *, struct utimbuf *);
static int fuse_open (const char *, struct fuse_file_info *);
static int fuse_read (const char *, char *, size_t, off_t, struct fuse_file_info *);
static int fuse_write (const char *, const char *, size_t, off_t, struct fuse_file_info *);
static int fuse_statfs (const char *, struct statvfs *);
static int fuse_flush (const char *, struct fuse_file_info *);
static int fuse_release (const char *, struct fuse_file_info *);
static int fuse_fsync (const char *, int, struct fuse_file_info *);
static int fuse_setxattr (const char *, const char *, const char *, size_t, int);
static int fuse_getxattr (const char *, const char *, char *, size_t);
static int fuse_listxattr (const char *, char *, size_t);
static int fuse_removexattr (const char *, const char *);
static int fuse_opendir (const char *, struct fuse_file_info *);
static int fuse_readdir (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *);
static int fuse_releasedir (const char *, struct fuse_file_info *);
static int fuse_fsyncdir (const char *, int, struct fuse_file_info *);
static void* fuse_op_init ();
static void fuse_op_destroy (void *);
static int fuse_access (const char *, int);
static int fuse_create (const char *, mode_t, struct fuse_file_info *);
static int fuse_ftruncate (const char *, off_t, struct fuse_file_info *);
static int fuse_fgetattr (const char *, struct stat *, struct fuse_file_info *);
static int fuse_lock (const char *, struct fuse_file_info *, int cmd, struct flock *);
static int fuse_utimens (const char *, const struct timespec tv[2]);
static int fuse_bmap (const char *, size_t blocksize, uint64_t *idx);
static int fuse_ioctl (const char *, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data);
static int fuse_poll (const char *, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp);
static int fuse_write_buf (const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *);
static int fuse_read_buf (const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *);
static int fuse_flock (const char *, struct fuse_file_info *, int op);
static int fuse_fallocate (const char *, int, off_t, off_t, struct fuse_file_info *);

static struct fuse_operations gFuseOp = {
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
    .opendir = fuse_opendir,
    .readdir = fuse_readdir,
    .releasedir = fuse_releasedir,
    .fsyncdir = fuse_fsyncdir,
    .init = fuse_op_init,
    .destroy = fuse_op_destroy,
    .access = fuse_access,
    .create = fuse_create,
    .ftruncate = fuse_ftruncate,
    .fgetattr = fuse_fgetattr,
};


int main(int argc, char* argv[])
{
    return fuse_main(argc, argv, &gFuseOp);
}

static int fuse_getattr (const char* path, struct stat* stat)
{
    syslog(LOG_INFO, "getattr, path: %s", path);

    if (0 == strcmp(path, "/") || 0 == strcmp(path, "/.") || 0 == strcmp(path, "/.Trash") || 0 == strcmp(path, "/.Trash-1000")) {
        stat->st_mode = S_IFDIR | 0755;
        stat->st_blksize = 1024;
        stat->st_blocks = 0;
        return 0;
    }

    return -ENOENT;
}

static int fuse_readlink (const char *, char *, size_t)
{
    syslog(LOG_INFO, "readlink");
    return 0;
}

static int fuse_getdir (const char *, fuse_dirh_t, fuse_dirfil_t)
{
    syslog(LOG_INFO, "getdir");
    return 0;
}

static int fuse_mknod (const char *, mode_t, dev_t)
{
    syslog(LOG_INFO, "mknod");
    return 0;
}

static int fuse_mkdir (const char *, mode_t)
{
    syslog(LOG_INFO, "mkdir");
    return 0;
}

static int fuse_unlink (const char *)
{
    syslog(LOG_INFO, "unlink");
    return 0;
}

static int fuse_rmdir (const char *)
{
    syslog(LOG_INFO, "rmdir");
    return 0;
}

static int fuse_symlink (const char *, const char *)
{
    syslog(LOG_INFO, "symlink");
    return 0;
}

static int fuse_rename (const char *, const char *)
{
    syslog(LOG_INFO, "rename");
    return 0;
}

static int fuse_link (const char *, const char *)
{
    syslog(LOG_INFO, "link");
    return 0;
}

static int fuse_chmod (const char *, mode_t)
{
    syslog(LOG_INFO, "chmod");
    return 0;
}

static int fuse_chown (const char *, uid_t, gid_t)
{
    syslog(LOG_INFO, "chown");
    return 0;
}

static int fuse_truncate (const char *, off_t)
{
    syslog(LOG_INFO, "truncate");
    return 0;
}

static int fuse_utime (const char *, struct utimbuf *)
{
    syslog(LOG_INFO, "utime");
    return 0;
}

static int fuse_open (const char *, struct fuse_file_info *)
{
    syslog(LOG_INFO, "open");
    return 0;
}

static int fuse_read (const char *, char *, size_t, off_t, struct fuse_file_info *)
{
    syslog(LOG_INFO, "read");
    return 0;
}

static int fuse_write (const char *, const char *, size_t, off_t, struct fuse_file_info *)
{
    syslog(LOG_INFO, "write");
    return 0;
}

static int fuse_statfs (const char *, struct statvfs *)
{
    syslog(LOG_INFO, "statfs");
    return 0;
}

static int fuse_flush (const char *, struct fuse_file_info *)
{
    syslog(LOG_INFO, "flush");
    return 0;
}

static int fuse_release (const char *, struct fuse_file_info *)
{
    syslog(LOG_INFO, "release");
    return 0;
}

static int fuse_fsync (const char *, int, struct fuse_file_info *)
{
    syslog(LOG_INFO, "fsync");
    return 0;
}

static int fuse_setxattr (const char *, const char *, const char *, size_t, int)
{
    syslog(LOG_INFO, "setxattr");
    return 0;
}

static int fuse_getxattr (const char *, const char *, char *, size_t)
{
    syslog(LOG_INFO, "getxattr");
    return 0;
}

static int fuse_listxattr (const char *, char *, size_t)
{
    syslog(LOG_INFO, "listxattr");
    return 0;
}

static int fuse_removexattr (const char *, const char *)
{
    syslog(LOG_INFO, "removexattr");
    return 0;
}

static int fuse_opendir (const char* path, struct fuse_file_info*)
{
    syslog(LOG_INFO, "opendir, path: %s", path);

    if (0 == strcmp(path, "/") || 0 == strcmp(path, "/.Trash") || 0 == strcmp(path, ".Trash-1000")) {
        return 0;
    }

    return -ENOENT;
}

static int fuse_readdir (const char* path, void* data, fuse_fill_dir_t entry, off_t offset, struct fuse_file_info *)
{
    syslog(LOG_INFO, "readdir, path: %s", path);

    if (0 == strcmp(path, "/")) {
        entry(data, ".", NULL, 0);
        entry(data, "..", NULL, 0);
        entry(data, ".Trash", NULL, 0);
        entry(data, ".Trash-1000", NULL, 0);
    }
    else {
        entry(data, ".", NULL, 0);
        entry(data, "..", NULL, 0);
    }

    return 0;
}

static int fuse_releasedir (const char * path, struct fuse_file_info *)
{
    syslog(LOG_INFO, "releasedir, path: %s", path);
    return 0;
}

static int fuse_fsyncdir (const char *, int, struct fuse_file_info *)
{
    syslog(LOG_INFO, "fsyncdir");
    return 0;
}

static void* fuse_op_init ()
{
    void* ff = malloc(1);
    syslog(LOG_INFO, "init, data: %p", ff);
    return ff;
}

static void fuse_op_destroy (void* privData)
{
    syslog(LOG_INFO, "destroy, data: %p", privData);
    if (privData) {
        free(privData);
    }
}

static int fuse_access (const char * path, int)
{
    syslog(LOG_INFO, "access, path: %s", path);

    if (0 == strcmp(path, "/") || 0 == strcmp(path, "/.Trash") || 0 == strcmp(path, ".Trash-1000")) {
        return 0;
    }

    return -ENOENT;
}

static int fuse_create (const char* path, mode_t, struct fuse_file_info *)
{
    syslog(LOG_INFO, "create, path: %s", path);

    return 0;
}

static int fuse_ftruncate (const char *, off_t, struct fuse_file_info *)
{
    syslog(LOG_INFO, "ftruncate");

    return 0;
}

static int fuse_fgetattr (const char *, struct stat *, struct fuse_file_info *)
{
    syslog(LOG_INFO, "fgetattr");

    return 0;
}

static int fuse_lock (const char *, struct fuse_file_info *, int cmd, struct flock *)
{
    syslog(LOG_INFO, "lock");

    return 0;
}

static int fuse_utimens (const char *, const struct timespec tv[2])
{
    syslog(LOG_INFO, "utimens");

    return 0;
}

static int fuse_bmap (const char *, size_t blocksize, uint64_t *idx)
{
    syslog(LOG_INFO, "bmap");

    return 0;
}

static int fuse_ioctl (const char *, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data)
{
    syslog(LOG_INFO, "ioctl");

    return 0;
}

static int fuse_poll (const char *, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp)
{
    syslog(LOG_INFO, "poll");

    return 0;
}

static int fuse_write_buf (const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *)
{
    syslog(LOG_INFO, "write_buf");

    return 0;
}

static int fuse_read_buf (const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *)
{
    syslog(LOG_INFO, "read_buf");

    return 0;
}

static int fuse_flock (const char *, struct fuse_file_info *, int op)
{
    syslog(LOG_INFO, "flock");

    return 0;
}

static int fuse_fallocate (const char *, int, off_t, off_t, struct fuse_file_info *)
{
    syslog(LOG_INFO, "fallocate");

    return 0;
}

