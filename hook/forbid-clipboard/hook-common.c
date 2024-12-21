#include <dlfcn.h>
#include <X11/X.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "hook-common.h"


static void* get_symbol(const char* funcName);


void*           gLibcHandle     = NULL;


typedef int (*HookChangeProperty)(void* /*Display*/dpy, int /*Window*/ w, int /*Atom*/ property, int /*Atom*/ type, int format, int mode, unsigned char* data, int nElements);


int hook_x_change_property(void* /*Display*/dpy, int /*Window*/ w, int /*Atom*/ property, int /*Atom*/ type, int format, int mode, unsigned char* data, int nElements)
{
    HookChangeProperty hc = get_symbol("XChangeProperty");

    //printf("prop: %s -- data: %s\n", XGetAtomName(dpy, property), data);

    x_clear_clipboard(dpy);

    return hc(dpy, w, property, type, format, mode, data, nElements);
}


void x_clear_clipboard (void* display)
{
    if (!display) {
        return;
    }

    Display* dsp = (Display*) display;

    Atom clip = XA_SECONDARY;
    XGetSelectionOwner (dsp, clip);
    XSetSelectionOwner (dsp, clip, None, CurrentTime);

    clip = XA_PRIMARY;
    XGetSelectionOwner (dsp, clip);
    XSetSelectionOwner (dsp, clip, None, CurrentTime);
    
    clip = XInternAtom (dsp, "CLIPBOARD", false);
    XGetSelectionOwner (dsp, clip);
    XSetSelectionOwner (dsp, clip, None, CurrentTime);
}

static void* get_symbol(const char* funcName)
{
    if (!funcName) { return NULL; }

    void* smb = dlsym(RTLD_NEXT, funcName);
    if (!smb && gLibcHandle) {
        smb = dlsym(gLibcHandle, funcName);
    }

    return smb;
}

