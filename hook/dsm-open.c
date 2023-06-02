/*************************************************************************
> FileName: dsm-open.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 30 May 2023 05:15:09 PM CST
 ************************************************************************/
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int xxxxxx_resolve_name2(int pid, char *name)
{
    char pbuf[200] = {0}, tmpBuf[100] = {0};
	int len;
    snprintf(tmpBuf, 99, "/proc/%d/exe", pid);
    int ret_len = readlinkat(100, tmpBuf, pbuf, 199);
    pbuf[ret_len] = '\0';
	len = sizeof(pbuf);
	bzero(name,len+1);
	memcpy(name,pbuf,len);
	return 0;
}

typedef int (*OpenPointer)(const char *pathname, int flags, ...);
int open (const char *pathname, int flags, ...)
{
    OpenPointer oldOpen = NULL;                             // open 的函数指针

    char programName[200] = {0};                            // FIXME:// 修复, 不能用固定长度

    int ret = 0;
    int mode = 0;                                           // open 第三个参数，打开方式
    int pid = 0;                                            // 进程 id

    va_list ap;
    va_start(ap, flags);
    mode = va_arg(ap, mode_t);
    va_end(ap);

    // 获得libc.so.6的句柄返回open函数在libc.so.6中的加载时的地址
    oldOpen = (OpenPointer) dlsym (RTLD_NEXT, "open");
    if (oldOpen == NULL) {
        syslog(LOG_ERR, "dlsym open error");
        return -1;
    }

    pid = getpid();

    xxxxxx_resolve_name2(pid, programName);

    if (strstr(pathname, ".dsm")) {
        syslog (LOG_ERR, "program: %s, path: %s", programName, pathname);
        printf ("program: %s, path: %s\n", programName, pathname);
        errno = EACCES;
        return -1;
    }

    
    if (mode == 0) {
        ret = oldOpen(pathname, flags);
    } else {
        ret = oldOpen(pathname, flags, mode);
    }

    return ret;
}
