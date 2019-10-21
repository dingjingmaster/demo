#include <stdio.h>


#include <errno.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include <errno.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/param.h>

#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX  MAXPATHLEN
#else
#define PATH_MAX  1024
#endif
#endif

extern int STD_LOOP_COUNT; /* changed by -in to set loop count to n */

extern long TEST_RETURN;
extern int TEST_ERRNO;

#define TEST(SCALL) \
        do { \
                errno = 0; \
                TEST_RETURN = SCALL; \
                TEST_ERRNO = errno; \
        } while (0)


#define LTP_ATTRIBUTE_NORETURN          __attribute__((noreturn))
#define LTP_ATTRIBUTE_UNUSED            __attribute__((unused))
#define LTP_ATTRIBUTE_UNUSED_RESULT     __attribute__((warn_unused_result))

#define TTYPE_MASK      0x3f
#define TPASS   0       /* Test passed flag */
#define TFAIL   1       /* Test failed flag */
#define TBROK   2       /* Test broken flag */
#define TWARN   4       /* Test warning flag */
#define TINFO   16      /* Test information flag */
#define TCONF   32      /* Test not appropriate for configuration flag */
#define TTYPE_RESULT(ttype)     ((ttype) & TTYPE_MASK)

#define TERRNO  0x100   /* Append errno information to output */
#define TTERRNO 0x200   /* Append TEST_ERRNO information to output */
#define TRERRNO 0x400   /* Capture errno information from TEST_RETURN to
                           output; useful for pthread-like APIs :). */


#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/* Round x to the next multiple of a.
 * a should be a power of 2.
 */
#define LTP_ALIGN(x, a)    __LTP_ALIGN_MASK(x, (typeof(x))(a) - 1)
#define __LTP_ALIGN_MASK(x, mask)  (((x) + (mask)) & ~(mask))

char* safe_basename(const char *file, const int lineno,
                    void (*cleanup_fn)(void), char *path);

int safe_chdir(const char *file, const int lineno,
               void (*cleanup_fn)(void), const char *path);

int safe_close(const char *file, const int lineno,
               void (*cleanup_fn)(void), int fildes);

int safe_creat(const char *file, const int lineno,
               void (*cleanup_fn)(void), const char *pathname, mode_t mode);

char* safe_dirname(const char *file, const int lineno,
                   void (*cleanup_fn)(void), char *path);

char* safe_getcwd(const char *file, const int lineno,
                  void (*cleanup_fn)(void), char *buf, size_t size);

struct passwd* safe_getpwnam(const char *file, const int lineno,
                             void (*cleanup_fn)(void), const char *name);

int safe_getrusage(const char *file, const int lineno,
                   void (*cleanup_fn)(void), int who, struct rusage *usage);

void* safe_malloc(const char *file, const int lineno,
                  void (*cleanup_fn)(void), size_t size);

int safe_mkdir(const char *file, const int lineno,
               void (*cleanup_fn)(void), const char *pathname, mode_t mode);

int safe_rmdir(const char *file, const int lineno,
               void (*cleanup_fn)(void), const char *pathname);

int safe_munmap(const char *file, const int lineno,
                void (*cleanup_fn)(void), void *addr, size_t length);

int safe_open(const char *file, const int lineno,
              void (*cleanup_fn)(void), const char *pathname, int oflags, ...);

int safe_pipe(const char *file, const int lineno,
              void (*cleanup_fn)(void), int fildes[2]);

ssize_t safe_read(const char *file, const int lineno,
                  void (*cleanup_fn)(void), char len_strict, int fildes,
                  void *buf, size_t nbyte);

ssize_t safe_pread(const char *file, const int lineno,
                   void (*cleanup_fn)(void), char len_strict,
                   int fildes, void *buf, size_t nbyte, off_t offset);

int safe_setegid(const char *file, const int lineno,
                 void (*cleanup_fn)(void), gid_t egid);

int safe_seteuid(const char *file, const int lineno,
                 void (*cleanup_fn)(void), uid_t euid);

int safe_setgid(const char *file, const int lineno,
                void (*cleanup_fn)(void), gid_t gid);

int safe_setuid(const char *file, const int lineno,
                void (*cleanup_fn)(void), uid_t uid);

int safe_getresuid(const char *file, const int lineno,
                   void (*cleanup_fn)(void),
                   uid_t *ruid, uid_t *euid, uid_t *suid);

int safe_getresgid(const char *file, const int lineno,
                   void (*cleanup_fn)(void),
                   gid_t *rgid, gid_t *egid, gid_t *sgid);

int safe_unlink(const char *file, const int lineno,
                void (*cleanup_fn)(void), const char *pathname);

int safe_link(const char *file, const int lineno,
              void (cleanup_fn)(void), const char *oldpath,
              const char *newpath);

int safe_linkat(const char *file, const int lineno,
                void (cleanup_fn)(void), int olddirfd, const char *oldpath,
                int newdirfd, const char *newpath, int flags);

ssize_t safe_readlink(const char *file, const int lineno,
                  void (cleanup_fn)(void), const char *path,
                  char *buf, size_t bufsize);

int safe_symlink(const char *file, const int lineno,
                 void (cleanup_fn)(void), const char *oldpath,
                 const char *newpath);

ssize_t safe_write(const char *file, const int lineno,
                   void (cleanup_fn)(void), char len_strict, int fildes,
                   const void *buf, size_t nbyte);

ssize_t safe_pwrite(const char *file, const int lineno,
                    void (cleanup_fn)(void), char len_strict, int fildes,
                    const void *buf, size_t nbyte, off_t offset);

long safe_strtol(const char *file, const int lineno,
                 void (cleanup_fn)(void), char *str, long min, long max);

unsigned long safe_strtoul(const char *file, const int lineno,
                           void (cleanup_fn)(void),
                           char *str, unsigned long min, unsigned long max);

long safe_sysconf(const char *file, const int lineno,
                  void (cleanup_fn)(void), int name);

int safe_chmod(const char *file, const int lineno, void (cleanup_fn)(void),
               const char *path, mode_t mode);

int safe_fchmod(const char *file, const int lineno, void (cleanup_fn)(void),
                int fd, mode_t mode);

int safe_chown(const char *file, const int lineno, void (cleanup_fn)(void),
               const char *path, uid_t owner, gid_t group);

int safe_fchown(const char *file, const int lineno, void (cleanup_fn)(void),
                int fd, uid_t owner, gid_t group);

pid_t safe_wait(const char *file, const int lineno, void (cleanup_fn)(void),
                int *status);

pid_t safe_waitpid(const char *file, const int lineno, void (cleanup_fn)(void),
                   pid_t pid, int *status, int opts);

int safe_kill(const char *file, const int lineno, void (cleanup_fn)(void),
              pid_t pid, int sig);

void *safe_memalign(const char *file, const int lineno,
                    void (*cleanup_fn)(void), size_t alignment, size_t size);

int safe_mkfifo(const char *file, const int lineno,
                void (*cleanup_fn)(void), const char *pathname, mode_t mode);

int safe_rename(const char *file, const int lineno, void (*cleanup_fn)(void),
                const char *oldpath, const char *newpath);

int safe_mount(const char *file, const int lineno, void (*cleanup_fn)(void),
               const char *source, const char *target,
               const char *filesystemtype, unsigned long mountflags,
               const void *data);

int safe_umount(const char *file, const int lineno, void (*cleanup_fn)(void),
                const char *target);

DIR* safe_opendir(const char *file, const int lineno, void (cleanup_fn)(void),
                  const char *name);

int safe_closedir(const char *file, const int lineno, void (cleanup_fn)(void),
                  DIR *dirp);

struct dirent *safe_readdir(const char *file, const int lineno,
                            void (cleanup_fn)(void),
                            DIR *dirp);

DIR* safe_opendir(const char *file, const int lineno,
                  void (cleanup_fn)(void),
                  const char *name);

struct dirent *safe_readdir(const char *file, const int lineno,
                            void (cleanup_fn)(void),
                            DIR *dirp);

int safe_closedir(const char *file, const int lineno,
                  void (cleanup_fn)(void),
                  DIR *dirp);

#define SAFE_FOPEN(cleanup_fn, path, mode) \
        safe_fopen(__FILE__, __LINE__, cleanup_fn, path, mode)

#define SAFE_FCLOSE(cleanup_fn, f) \
        safe_fclose(__FILE__, __LINE__, cleanup_fn, f)

#define SAFE_ASPRINTF(cleanup_fn, strp, fmt, ...) \
        safe_asprintf(__FILE__, __LINE__, cleanup_fn, strp, fmt, __VA_ARGS__)

#define SAFE_POPEN(cleanup_fn, command, type) \
        safe_popen(__FILE__, __LINE__, cleanup_fn, command, type)

#ifndef EOWNERDEAD
#define EOWNERDEAD      ESRCH
#endif
#ifndef ENOTRECOVERABLE
#define ENOTRECOVERABLE EBADR
#endif

#define PASS    0
#define FAIL    1
#define UNRESOLVED 2

#define DPRINTF(a, x, args...)     fprintf(a, x , ##args);
#define EPRINTF(x, args...)     fprintf(stderr, "%s: %d: " x "\n",__FILE__, __LINE__, ##args);

static void setup(void);
static void cleanup(void);

char *TCID = "mount02";

#define DIR_MODE        (S_IRWXU | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP)
#define FILE_MODE       (S_IRWXU | S_IRWXG | S_IRWXO)

static char path[PATH_MAX + 2];
static const char *long_path = path;
static const char *fs_type;
static const char *wrong_fs_type = "error";
static const char *mntpoint = "mntpoint";
static const char *device;
static const char *null = NULL;
static const char *fault = (void*)-1;
static const char *nonexistent = "nonexistent";
static const char *char_dev = "char_device";
static const char *file = "filename";
static int fd;


static void do_umount(void);
static void close_umount(void);
static void do_mount(void);
static void mount_open(void);

static struct test_case {
        const char **device;
        const char **mntpoint;
        const char **fs_type;
        unsigned long flag;
        int exp_errno;
        void (*setup)(void);
        void (*cleanup)(void);
} tc[] = {
        {&device, &mntpoint, &wrong_fs_type, 0, ENODEV, NULL, NULL},
        {&char_dev, &mntpoint, &fs_type, 0, ENOTBLK, NULL, NULL},
        {&device, &mntpoint, &fs_type, 0, EBUSY, do_mount, do_umount},
        {&device, &mntpoint, &fs_type, MS_REMOUNT | MS_RDONLY, EBUSY,
         mount_open, close_umount},
        {&null, &mntpoint, &fs_type, 0, EINVAL, NULL, NULL},
        {&device, &mntpoint, &null, 0, EINVAL, NULL, NULL},
        {&device, &mntpoint, &fs_type, MS_REMOUNT, EINVAL, NULL, NULL},
        {&fault, &mntpoint, &fs_type, 0, EFAULT, NULL, NULL},
        {&device, &mntpoint, &fault, 0, EFAULT, NULL, NULL},
        {&device, &long_path, &fs_type, 0, ENAMETOOLONG, NULL, NULL},
        {&device, &nonexistent, &fs_type, 0, ENOENT, NULL, NULL},
        {&device, &file, &fs_type, 0, ENOTDIR, NULL, NULL},
};

int TST_TOTAL = ARRAY_SIZE(tc);

static void verify_mount(struct test_case *tc)
{
        if (tc->setup)
                tc->setup();

        TEST(mount(*tc->device, *tc->mntpoint, *tc->fs_type, tc->flag, NULL));

        if (TEST_RETURN != -1) {
                tst_resm(TFAIL, "mount() succeded unexpectedly (ret=%li)",
                         TEST_RETURN);
                goto cleanup;
        }

        if (TEST_ERRNO != tc->exp_errno) {
                tst_resm(TFAIL | TTERRNO,
                         "mount() was expected to fail with %s(%i)",
                         tst_strerrno(tc->exp_errno), tc->exp_errno);
                goto cleanup;
        }

        tst_resm(TPASS | TTERRNO, "mount() failed expectedly");

cleanup:
        if (tc->cleanup)
                tc->cleanup();
}

int main(int ac, char **av)
{
        int lc, i;

        tst_parse_opts(ac, av, NULL, NULL);

        setup();

        for (lc = 0; TEST_LOOPING(lc); lc++) {
                tst_count = 0;

                for (i = 0; i < TST_TOTAL; ++i)
                        verify_mount(tc + i);
        }

        cleanup();
        tst_exit();
}


static void do_mount(void)
{
        if (mount(device, mntpoint, fs_type, 0, NULL))
                tst_brkm(TBROK | TERRNO, cleanup, "Failed to mount(mntpoint)");
}

static void mount_open(void)
{
        do_mount();

        fd = SAFE_OPEN(cleanup, "mntpoint/file", O_CREAT | O_RDWR, S_IRWXU);
}

static void close_umount(void)
{
        SAFE_CLOSE(cleanup, fd);
        do_umount();
}

static void do_umount(void)
{
        if (tst_umount(mntpoint))
                tst_brkm(TBROK | TERRNO, cleanup, "Failed to umount(mntpoint)");
}


static void setup(void)
{
        dev_t dev;

        tst_sig(FORK, DEF_HANDLER, cleanup);

        tst_require_root();

        tst_tmpdir();

        SAFE_TOUCH(cleanup, file, FILE_MODE, NULL);

        fs_type = tst_dev_fs_type();
        device = tst_acquire_device(cleanup);

        if (!device)
                tst_brkm(TCONF, cleanup, "Failed to obtain block device");

        tst_mkfs(cleanup, device, fs_type, NULL, NULL);

        SAFE_MKDIR(cleanup, mntpoint, DIR_MODE);

        memset(path, 'a', PATH_MAX + 1);

        dev = makedev(1, 3);
        if (mknod(char_dev, S_IFCHR | FILE_MODE, dev)) {
                tst_brkm(TBROK | TERRNO, cleanup,
                         "failed to mknod(char_dev, S_IFCHR | FILE_MODE, %lu)",
                         dev);
        }

        TEST_PAUSE;
}

static void cleanup(void)
{
        if (device)
                tst_release_device(device);

        tst_rmdir();
}
