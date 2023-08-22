/*************************************************************************
> FileName: x-get-pid-by-win-id.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 22 Aug 2023 09:21:45 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static void print_window_pid (Display* display, Window window);

int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;

    display = XOpenDisplay (0);
    rootWindow = 0x3400001; //DefaultRootWindow (display);

    //
    print_window_pid (display, rootWindow);

    XCloseDisplay(display);

    return 0;
}

static void print_window_pid (Display* display, Window window)
{
    if (!display) return;

    Atom pidAtom = XInternAtom (display, "_NET_WM_PID", false);
    Atom pidActualType = 0;
    int actualFormatRet = 0;
    unsigned long nItemRet = 0;
    unsigned long byteAfterRet = 0;
    unsigned char* propRet = NULL;

    XGetWindowProperty (display, window, pidAtom, 0, 1, false, XA_CARDINAL, &pidActualType, &actualFormatRet, &nItemRet, &byteAfterRet, &propRet);

    char* actualType = XGetAtomName(display, pidActualType);
    printf ("actual type: %s\n", actualType ? actualType : "<null>");

        printf ("%u\n", *((uint32_t*)(propRet)));
    for (int i = 0; propRet && propRet[i]; ++i) {
    }
}
