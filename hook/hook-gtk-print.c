/*************************************************************************
> FileName: hook-gtk-print.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月29日 星期四 14时13分01秒
 ************************************************************************/
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>


typedef void (*GtkBeginPrintCB) (void* o, void* h, void* n);

static void* gtkPrintHandle = NULL;
GtkBeginPrintCB gtkBeginPrintCB = NULL;


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
            return 0;
        }
        return pp (ins, detail, cb, data, destroyData, flags);
    }

    return 0;
}
