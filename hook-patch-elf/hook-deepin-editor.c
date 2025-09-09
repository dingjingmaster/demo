/*************************************************************************
> FileName: hook-deepin-editor.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年09月09日 星期二 11时17分41秒
 ************************************************************************/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEBUG

#ifdef DEBUG
#include <syslog.h>
#define DLOG(...) syslog(LOG_ERR, __VA_ARGS__)
#else
#define DLOG(...)
#endif

#ifndef RTLD_NEXT
#define RTLD_NEXT ((void*) -1l)
#endif


#ifdef __cplusplus
extern "C" 
{
#endif

typedef void* (*Orig) (void const * sender, char const* sig, void const* rec, char const* val, int type);

void* _ZN7QObject7connectEPKS_PKcS1_S3_N2Qt14ConnectionTypeE(void const * sender, char const* sig, void const* rec, char const* val, int type)
{
    Orig orig = (Orig) dlsym(RTLD_DEFAULT, "_ZN7QObject7connectEPKS_PKcS1_S3_N2Qt14ConnectionTypeE");
    //DLOG("signal: %s", sig);

    return orig(sender, sig, rec, val, type);
}

#if 0
typedef void (*QActionTriggered) (bool);

void _ZN7QAction9triggeredEb(void* self, bool b)
{
    DLOG("triggered, %p", self);
    return;
    QActionTriggered orig = (QActionTriggered) dlsym(RTLD_DEFAULT, "_ZN7QAction9triggeredEb");
    if (orig) {
        DLOG("Found function!");

        orig(b);
    }
}
#endif

#ifdef __cplusplus
}
#endif

#if 0
#include "hook-func.h"


typedef void (*DeepinPopupDialog) ();

void hook_deepin_editor_init() __attribute__((constructor()));
void hook_deepin_editor_clean() __attribute__((destructor()));

extern void _ZN6Window16popupPrintDialogEv ();
static void hook_deepin_editor_popup_printer_dlg ();

static HookFunc* gsHookFunc = NULL;
DeepinPopupDialog gsDlgPopup = NULL;//_ZN6Window16popupPrintDialogEv;


void hook_deepin_editor_init() 
{
    gsDlgPopup = (DeepinPopupDialog) dlsym(RTLD_DEFAULT, "_ZN6Window16popupPrintDialogEv");
    if (!gsDlgPopup) {
        printf ("dlsym error\n");
        return;
    }

    gsHookFunc = hook_func_create();
    if (!gsHookFunc) {
        printf ("hook_func_create()\n");
        return;
    }

    // void _ZN6Window16popupPrintDialogEv ()
    int ret = hook_func_prepare(gsHookFunc, (void**) &gsDlgPopup, (void*) hook_deepin_editor_popup_printer_dlg);
    if (0 != ret) {
        printf ("hook_func_prepare error!\n");
        return;
    }

    ret = hook_func_install(gsHookFunc, 0);
    if (0 != ret) {
        printf ("hook_func_install error!\n");
        return;
    }
}

void hook_deepin_editor_clean()
{
    if (gsHookFunc) {
        int ret = hook_func_uninstall(gsHookFunc, 0);
        if (0 != ret) {
            printf ("hook_func_uninstall error!\n");
            return;
        }
        ret = hook_func_destroy(&gsHookFunc);
        if (0 != ret) {
            printf ("hook_func_destroy error!\n");
            return;
        }
    }
}


static void hook_deepin_editor_popup_printer_dlg ()
{
    printf("Hook In -----\n");
}
#endif
