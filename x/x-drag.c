/*************************************************************************
> FileName: x-drag.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 30 Oct 2023 06:27:38 PM CST
 ************************************************************************/
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    Display* dsp = XOpenDisplay(NULL);
    Window rootWin = DefaultRootWindow(dsp);

    XEvent ev;
    XGrabPointer(dsp, rootWin, False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

    while (1) {
        XNextEvent(dsp, &ev);
        if (ev.type == ButtonRelease) {
            Atom acType;
            int acFormat;
            unsigned long itemCount;
            unsigned long bytesAfter;
            unsigned char* data = NULL;

            XGetWindowProperty(dsp, rootWin, XA_PRIMARY, 0, 0, False, AnyPropertyType, &acType, &acFormat, &itemCount, &bytesAfter, &data);
            if (NULL != data) {
                printf ("%s\n", (char*) data);
                XFree(data);
            }
        }
    }

    return 0;

    (void) argc;
    (void) argv;
}
