/*************************************************************************
> FileName: hook-screenshot.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月24日 星期四 13时42分04秒
 ************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>


static void get_program_full_path (char buf[], int bufLen)
{
    char pBuf[128] = {0};

    if (snprintf(pBuf, sizeof(pBuf) - 1, "/proc/%d/exe", getpid()) > 0) {
        if (readlink(pBuf, buf, bufLen - 1) < 0) {
            syslog(LOG_ERR, "readlink error: %s", strerror(errno));
        }
    }
}

#if 0
void gdk_pixbuf_save_to_stream_async (void* pixbuf, void* stream, const char* type, void* cancellable, void* callback, void* udata, ...)
{
	printf("no screenshot\n");
	exit(0);
}
#endif

typedef void* (*GDbusConnectionCallSync)(void* conn, const char* busName, const char* objPath, const char* interName, const char* methName, void* parameters, const void* replyType, int flags, int timeoutSec, void** error);
void* g_dbus_connection_call_sync(void* conn, const char* busName, const char* objPath, const char* interName, const char* methName, void* parameters, const void* replyType, int flags, int timeoutSec, void** error)
{
    GDbusConnectionCallSync orig = (GDbusConnectionCallSync) dlsym(RTLD_NEXT, "g_dbus_connection_call_sync");
    if (!orig) {
        syslog(LOG_ERR, "get g_dbus_connection_call_sync error!");
        return NULL;
    }

    char buf[1024] = {0};
    get_program_full_path(buf, sizeof(buf) - 1);

    syslog(LOG_ERR, "[gdbs call sync] %s, busName: %s, obj path: %s, inter name: %s, method name: %s", buf, busName, objPath, interName, methName);

    return orig(conn, busName, objPath, interName, methName, parameters, replyType, flags, timeoutSec, error);
}


typedef void (*GDbusConnectionCall) (void* conn, const char* busName, const char* objPath, const char* interName, const char* methName, void* parameters, const void* replyType, int flags, int timeoutSec, void* cancelable, void* cb, void* uData);

void g_dbus_connection_call (void* conn, const char* busName, const char* objPath, const char* interName, const char* methName, void* parameters, const void* replyType, int flags, int timeoutSec, void* cancelable, void* cb, void* uData)
{
    GDbusConnectionCall orig = (GDbusConnectionCall) dlsym(RTLD_NEXT, "g_dbus_connection_call");
    if (!orig) {
        syslog(LOG_ERR, "get g_dbus_connection_call error!");
        return;
    }

    char buf[1024] = {0};
    get_program_full_path(buf, sizeof(buf) - 1);

    syslog(LOG_ERR, "[gdbs call] %s, busName: %s, obj path: %s, inter name: %s, method name: %s", buf, busName, objPath, interName, methName);

    orig(conn, busName, objPath, interName, methName, parameters, replyType, flags, timeoutSec, cancelable, cb, uData);
}





