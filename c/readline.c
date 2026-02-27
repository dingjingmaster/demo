/*************************************************************************
> FileName: readline.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年02月27日 星期五 11时26分52秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <syscall.h>

static inline int get_line (int fd, char buf[], int bufLen)
{
    int i = 0;
    for (i = 0; i < bufLen; ++i) {
        ssize_t r = syscall(__NR_read, fd, &buf[i], 1);
//        printf("%ld '%c' %s\n", r, buf[i], strerror(errno));
        if (r <= 0 || buf[i] == '\0' || buf[i] == '\n') {
            break;
        }
    }
    buf[i] = '\0';

    return i;
}

int64_t hook_common_read_proc_name(char *procName, int32_t procNameLen)
{
    if (!procName || procNameLen <= 0) {
        return -1;
    }

    memset(procName, 0, procNameLen);

    char procNameBuf[64] = {0};
    snprintf(procNameBuf, sizeof(procNameBuf) - 1, "/proc/%d/exe", getpid());

    const ssize_t retLen = readlink(procNameBuf, procName, procNameLen - 1);
    int nameS = 0;
    int nameLen = 0;

    for (int i = retLen - 1; i > 0; --i) {
        printf ("%c ", procName[i]);
        if (procName[i] == '/') {
            nameS = i + 1;
            nameLen = retLen - nameS;
            break;
        }
    }
    printf ("\n");

    if (nameS > 0) {
        int i = 0;
        printf ("name len: %d(%d) - %d\n", nameLen, nameS, retLen);
        for (i = 0; i < nameLen; ++i) {
            procName[i] = procName[i + nameS];
        }
        procName[i] = '\0';
    }
    

    return retLen;
}

int main() 
{
    int fd = 0;
    int len = 0;
    char line[32] = {0};
    char procName[4096] = {0};
    const char *filename = "/sys/fs/safe_fs/encrypt_process";

    if (hook_common_read_proc_name(procName, sizeof(procName) - 1) <= 0) {
        printf("Error read proc name\n");
        return 0;
    }

    printf ("proc: %s\n", procName);


    fd = syscall(__NR_openat, -1, filename, O_RDONLY);
    if (fd < 0) {
        printf("open error\n");
        return 1;
    }

    while ((len = get_line(fd, line, sizeof(line) - 1)) > 0) {
        // 去掉换行符
        printf("%s\n", line);
    }

    syscall(__NR_close, fd);

    return 0;
}
