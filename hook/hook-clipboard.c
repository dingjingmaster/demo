/*************************************************************************
> FileName: hook-gtk_clipboard.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Jul 2023 08:16:57 AM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common-hook.h"

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif

typedef void (*GtkClipboardRequestUrisPtr) (void* clip, void* cb/*void(*)(GtkClipboard*,char**uris,void*udata)*/, void* udata);
void gtk_clipboard_request_uris (void* clipboard, void* cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
    GtkClipboardRequestUrisPtr old = (GtkClipboardRequestUrisPtr) get_system_function_ptr ("gtk_clipboard_request_uris");
    if (NULL == old) {
        return;
    }

    syslog(LOG_ERR, "OK clipboard!");

    void(*cb1)(void*, void**, void*) = get_common_function_ptr ("common_gtk_clipboard_uri_received");
    if (NULL == cb1) {
        syslog (LOG_ERR, "common_gtk_clipboard_uri_received get error!");
        return;
    }

    old (clipboard, cb1, udata);


    return old (clipboard, cb, udata);
}

typedef void (*GtkClipboardRequestTextPtr) (void* clip, void* cb, void* udata);
void gtk_clipboard_request_text (void* /*GtkClipboard**/ clip, void* /*GtkClipboardTextReceivedFunc*/ cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
    GtkClipboardRequestTextPtr old = (GtkClipboardRequestTextPtr) get_system_function_ptr ("gtk_clipboard_request_text");
    if (NULL == old) {
        return;
    }

    syslog(LOG_ERR, "OK clipboard!");

    void(*cb1)(void*, void*, void*) = get_common_function_ptr ("common_gtk_clipboard_text_received");
    if (NULL == cb1) {
        syslog (LOG_ERR, "common_gtk_clipboard_text_received get error!");
        return;
    }

    old (clip, cb1, udata);


    return old (clip, cb, udata);
}

typedef void (*GtkClipboardRequestTargets) (void*, void* cb, void* udata);
void gtk_clipboard_request_targets (void* clip, void* cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
}

typedef void (*GtkClipboardRequestRichText) (void*, void* cb, void* udata);
void gtk_clipboard_request_rich_text (void* clip, void* cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
}

void gtk_clipboard_request_contents (void* clip, int target, void* cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
}

void gdk_clipboard_read_texture_async (void* clip, void* cancel, void* cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
}

void gdk_clipboard_read_text_async (void* clip, void* cancel, void* cb, void* udata)
{
    syslog(LOG_ERR, "%s", __FUNCTION__);
}

int XGetWindowProperty (void* display, int64_t window, int64_t property, long longoffset, long longLen, bool delete, int64_t reqType, int64_t* actualType, int* actualFormatRet, unsigned long* nitemsRet, unsigned long* bytesAfterRet, unsigned char** propRet)
{
    int(*old)(void*, int64_t, int64_t, long, long, bool, int64_t, int64_t*, int*, unsigned long*, unsigned long* unsigned char**) = get_system_function_ptr ("XGetWindowProperty");

    ret = old (display, window, property, longoffset, longLen, delete, reqType, actualType, actualFormatRet, nitemsRet, bytesAfterRet, propRet);

    return ret;
}
