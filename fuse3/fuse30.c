//
// Created by dingjing on 25-4-10.
//
#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>


static int fuse_op_getattr (const char *, struct stat *, struct fuse_file_info *fi);
static int fuse_op_readlink (const char *, char *, size_t);
static int fuse_op_mknod (const char *, mode_t, dev_t);
static int fuse_op_mkdir (const char *, mode_t);
static int fuse_op_unlink (const char *);
static int fuse_op_rmdir (const char *);
static int fuse_op_symlink (const char *, const char *);
static int fuse_op_rename (const char *, const char *, unsigned int flags);
static int fuse_op_link (const char *, const char *);
static int fuse_op_chmod (const char *, mode_t, struct fuse_file_info *fi);
static int fuse_op_chown (const char *, uid_t, gid_t, struct fuse_file_info *fi);
static int fuse_op_truncate (const char *, off_t, struct fuse_file_info *fi);
static int fuse_op_open (const char *, struct fuse_file_info *);
static int fuse_op_read (const char *, char *, size_t, off_t, struct fuse_file_info *);
static int fuse_op_write (const char *, const char *, size_t, off_t, struct fuse_file_info *);
static int fuse_op_statfs (const char *, struct statvfs *);
static int fuse_op_flush (const char *, struct fuse_file_info *);
static int fuse_op_release (const char *, struct fuse_file_info *);
static int fuse_op_fsync (const char *, int, struct fuse_file_info *);
static int fuse_op_setxattr (const char *, const char *, const char *, size_t, int);
static int fuse_op_getxattr (const char *, const char *, char *, size_t);
static int fuse_op_listxattr (const char *, char *, size_t);
static int fuse_op_removexattr (const char *, const char *);
static int fuse_op_opendir (const char *, struct fuse_file_info *);
static int fuse_op_readdir (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *, enum fuse_readdir_flags);
static int fuse_op_releasedir (const char *, struct fuse_file_info *);
static int fuse_op_fsyncdir (const char *, int, struct fuse_file_info *);
static void* fuse_op_init (struct fuse_conn_info *conn, struct fuse_config *cfg);
static void fuse_op_destroy (void *private_data);
static int fuse_op_access (const char *, int);
static int fuse_op_create (const char *, mode_t, struct fuse_file_info *);
static int fuse_op_lock (const char *, struct fuse_file_info *, int cmd, struct flock *);
static int fuse_op_utimens (const char *, const struct timespec tv[2], struct fuse_file_info *fi);
static int fuse_op_bmap (const char *, size_t blocksize, uint64_t *idx);
static int fuse_op_ioctl (const char *, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data);
static int fuse_op_poll (const char *, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp);
static int fuse_op_write_buf (const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *);
static int fuse_op_read_buf (const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *);
static int fuse_op_flock (const char *, struct fuse_file_info *, int op);
static int fuse_op_fallocate (const char *, int, off_t, off_t, struct fuse_file_info *);
static ssize_t fuse_op_copy_file_range (const char *path_in, struct fuse_file_info *fi_in, off_t offset_in, const char *path_out, struct fuse_file_info *fi_out, off_t offset_out, size_t size, int flags);
static off_t fuse_op_lseek (const char *, off_t off, int whence, struct fuse_file_info *);


struct fuse_operations gFileOp = {
    .getattr = fuse_op_getattr,
    .readlink = fuse_op_readlink,
    .mknod = fuse_op_mknod,
    .mkdir = fuse_op_mkdir,
    .unlink = fuse_op_unlink,
    .rmdir = fuse_op_rmdir,
    .symlink = fuse_op_symlink,
    .rename = fuse_op_rename,
    .link = fuse_op_link,
    .chmod = fuse_op_chmod,
    .chown = fuse_op_chown,
    .truncate = fuse_op_truncate,
    .open = fuse_op_open,
    .read = fuse_op_read,
    .write = fuse_op_write,
    .statfs = fuse_op_statfs,
    .flush = fuse_op_flush,
    .release = fuse_op_release,
    .fsync = fuse_op_fsync,
    .setxattr = fuse_op_setxattr,
    .getxattr = fuse_op_getxattr,
    .listxattr = fuse_op_listxattr,
    .removexattr = fuse_op_removexattr,
    .opendir = fuse_op_opendir,
    .readdir = fuse_op_readdir,
    .releasedir = fuse_op_releasedir,
    .fsyncdir = fuse_op_fsyncdir,
    .init= fuse_op_init,
    .destroy = fuse_op_destroy,
    .access = fuse_op_access,
    .create = fuse_op_create,
    .lock = fuse_op_lock,
    .utimens = fuse_op_utimens,
    .bmap = fuse_op_bmap,
    .ioctl = fuse_op_ioctl,
    .poll = fuse_op_poll,
    .write_buf = fuse_op_write_buf,
    .read_buf = fuse_op_read_buf,
    .flock = fuse_op_flock,
    .fallocate = fuse_op_fallocate,
    .copy_file_range = fuse_op_copy_file_range,
    .lseek = fuse_op_lseek,
};


int main (int argc, char *argv[])
{
    void* p = malloc(1);
    syslog(LOG_ERR, "addr: %p\n", p);
    return fuse_main(argc, argv, &gFileOp, p);
}

static int fuse_op_getattr (const char * path, struct stat * statBuf, struct fuse_file_info *fi)
{
    syslog(LOG_ERR, "getattr, path: %s", path);

    if (0 == strcmp(path, "/") || 0 == strcmp(path, "/.") || 0 == strcmp(path, "/.Trash") || 0 == strcmp(path, "/.Trash-1000")) {
        statBuf->st_blksize = 4096;
        statBuf->st_blocks = 1;
        statBuf->st_size = 4096;
        statBuf->st_mode = S_IFDIR | 0755;
    }

    return 0;
}

static int fuse_op_readlink (const char *, char *, size_t)
{
    syslog(LOG_ERR, "readlink");

    return 0;
}

static int fuse_op_mknod (const char *, mode_t, dev_t)
{
    syslog(LOG_ERR, "mknod");

    return 0;
}

static int fuse_op_mkdir (const char *, mode_t)
{
    syslog(LOG_ERR, "mkdir");

    return 0;
}

static int fuse_op_unlink (const char *)
{
    syslog(LOG_ERR, "unlink");

    return 0;
}

static int fuse_op_rmdir (const char *)
{
    syslog(LOG_ERR, "rmdir");

    return 0;
}

static int fuse_op_symlink (const char *, const char *)
{
    syslog(LOG_ERR, "symlink");

    return 0;
}

static int fuse_op_rename (const char *, const char *, unsigned int flags)
{
    syslog(LOG_ERR, "rename");

    return 0;
}

static int fuse_op_link (const char *, const char *)
{
    syslog(LOG_ERR, "link");

    return 0;
}

static int fuse_op_chmod (const char *, mode_t, struct fuse_file_info *fi)
{
    syslog(LOG_ERR, "chmod");

    return 0;
}

static int fuse_op_chown (const char *, uid_t, gid_t, struct fuse_file_info *fi)
{
    syslog(LOG_ERR, "chown");

    return 0;
}

static int fuse_op_truncate (const char *, off_t, struct fuse_file_info *fi)
{
    syslog(LOG_ERR, "truncate");

    return 0;
}

static int fuse_op_open (const char *, struct fuse_file_info *)
{
    syslog(LOG_ERR, "open");

    return 0;
}

static int fuse_op_read (const char *, char *, size_t, off_t, struct fuse_file_info *)
{
    syslog(LOG_ERR, "read");

    return 0;
}

static int fuse_op_write (const char *, const char *, size_t, off_t, struct fuse_file_info *)
{
    syslog(LOG_ERR, "write");

    return 0;
}

static int fuse_op_statfs (const char *, struct statvfs *)
{
    syslog(LOG_ERR, "statfs");

    return 0;
}

static int fuse_op_flush (const char *, struct fuse_file_info *)
{
    syslog(LOG_ERR, "flush");

    return 0;
}

static int fuse_op_release (const char *, struct fuse_file_info *)
{
    syslog(LOG_ERR, "release");

    return 0;
}

static int fuse_op_fsync (const char *, int, struct fuse_file_info *)
{
    syslog(LOG_ERR, "fsync");

    return 0;
}

static int fuse_op_setxattr (const char *, const char *, const char *, size_t, int)
{
    syslog(LOG_ERR, "setxattr");

    return 0;
}

static int fuse_op_getxattr (const char *, const char *, char *, size_t)
{
    syslog(LOG_ERR, "getxattr");

    return 0;
}

static int fuse_op_listxattr (const char *, char *, size_t)
{
    syslog(LOG_ERR, "listxattr");

    return 0;
}

static int fuse_op_removexattr (const char *, const char *)
{
    syslog(LOG_ERR, "removexattr");

    return 0;
}

static int fuse_op_opendir (const char *, struct fuse_file_info *)
{
    syslog(LOG_ERR, "openxattr");

    return 0;
}

static int fuse_op_readdir (const char * path, void * data, fuse_fill_dir_t entry, off_t, struct fuse_file_info *, enum fuse_readdir_flags)
{
    syslog(LOG_ERR, "readdir");

    if (0 == strcmp(path, "/")) {
        entry(data, ".", NULL, 0, 0);
        entry(data, "..", NULL, 0, 0);
        entry(data, ".Trash", NULL, 0, 0);
        entry(data, ".Trash-1000", NULL, 0, 0);
    }
    else {
        entry(data, ".", NULL, 0, 0);
        entry(data, "..", NULL, 0, 0);
    }

    return 0;
}

static int fuse_op_releasedir (const char *, struct fuse_file_info *)
{
    syslog(LOG_ERR, "releasedir");

    return 0;
}

static int fuse_op_fsyncdir (const char *, int, struct fuse_file_info *)
{
    syslog(LOG_ERR, "fsyncdir");

    return 0;
}

static void* fuse_op_init (struct fuse_conn_info *conn, struct fuse_config *cfg)
{
    syslog(LOG_ERR, "init");

    void* p = fuse_get_context()->private_data;
    syslog(LOG_ERR,"addr: %p\n", p);

    return p;
}

static void fuse_op_destroy (void *private_data)
{
    syslog(LOG_ERR, "destroy");
}

static int fuse_op_access (const char *, int)
{
    syslog(LOG_ERR, "access");

    return 0;
}

static int fuse_op_create (const char *, mode_t, struct fuse_file_info *)
{
    syslog(LOG_ERR, "create");

    return 0;
}

static int fuse_op_lock (const char *, struct fuse_file_info *, int cmd, struct flock *)
{
    syslog(LOG_ERR, "lock");

    return 0;
}

static int fuse_op_utimens (const char *, const struct timespec tv[2], struct fuse_file_info *fi)
{
    syslog(LOG_ERR, "utimens");

    return 0;
}

static int fuse_op_bmap (const char *, size_t blocksize, uint64_t *idx)
{
    syslog(LOG_ERR, "bmap");

    return 0;
}

static int fuse_op_ioctl (const char *, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data)
{
    syslog(LOG_ERR, "ioctl");

    return 0;
}

static int fuse_op_poll (const char *, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp)
{
    syslog(LOG_ERR, "poll");

    return 0;
}

static int fuse_op_write_buf (const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *)
{
    syslog(LOG_ERR, "write_buf");

    return 0;
}

static int fuse_op_read_buf (const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *)
{
    syslog(LOG_ERR, "read_buf");

    return 0;
}

static int fuse_op_flock (const char *, struct fuse_file_info *, int op)
{
    syslog(LOG_ERR, "flock");

    return 0;
}

static int fuse_op_fallocate (const char *, int, off_t, off_t, struct fuse_file_info *)
{
    syslog(LOG_ERR, "fallocate");

    return 0;
}

static ssize_t fuse_op_copy_file_range (const char *path_in, struct fuse_file_info *fi_in, off_t offset_in, const char *path_out, struct fuse_file_info *fi_out, off_t offset_out, size_t size, int flags)
{
    syslog(LOG_ERR, "copy_file_range");

    return 0;
}

static off_t fuse_op_lseek (const char *, off_t off, int whence, struct fuse_file_info *)
{
    syslog(LOG_ERR, "lseek");

    return 0;
}


