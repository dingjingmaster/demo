/*************************************************************************
> FileName: hook-gtk-print.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月29日 星期四 14时13分01秒
 ************************************************************************/
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>


static bool isGedit = false;
static void* gtkPrintHandle = NULL;


typedef void (*GeditBeginPrintCB) (void* op, void* context, void* job);
static GeditBeginPrintCB geditBeginPrintCB = NULL;
static void gedit_begin_print_cb (void* operation, void* context, void* job)
{
    char file[4096] = {0};

}


static void get_program_full_path (char buf[], int bufLen)
{
        char pBuf[128] = {0};

        if (snprintf(pBuf, sizeof(pBuf) - 1, "/proc/%d/exe", getpid()) > 0) {
                if (readlink(pBuf, buf, bufLen - 1) < 0) {
                        syslog(LOG_ERR, "readlink error: %s", strerror(errno));
                }
        }
}


typedef void* (*GtkPrintOperationNew)();
void* gtk_print_operation_new()
{
    GtkPrintOperationNew pp = dlsym (RTLD_NEXT, "gtk_print_operation_new");
    if (pp) {
        gtkPrintHandle = pp ();
        return gtkPrintHandle;
    }

    return NULL;
}

typedef unsigned long (*GSignalConnectData) (void* ins, const char* detail, void* cb, void* data, void* destroyData, int flags);
unsigned long g_signal_connect_data (void* ins, const char* detail, void* cb, void* data, void* destroyData, int flags)
{
    GSignalConnectData pp = dlsym (RTLD_NEXT, "g_signal_connect_data");
    if (pp) {
        if ((ins == gtkPrintHandle) && (!strcmp(detail, "begin-print"))) {
            syslog(LOG_ERR, "begin-print");
            char proc[4096] = {0};
            get_program_full_path(proc, sizeof(proc) - 1);
            if (strstr(proc, "/gedit")) {
                isGedit = true;
                geditBeginPrintCB = cb;
                return pp (ins, detail, gedit_begin_print_cb, data, destroyData, flags);
            }
            return 0;
        }
        return pp (ins, detail, cb, data, destroyData, flags);
    }

    return 0;
}
