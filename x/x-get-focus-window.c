/*************************************************************************
> FileName: x-get-focus-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 21 Aug 2023 11:28:26 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

static Window getFocusedWindow(Display* display);

int main ()
{
    Display* display = XOpenDisplay(NULL);
    Window win = getFocusedWindow(display);

    printf ("focused window ID: %lu\n", win);

    int nprops = 0;
    Atom* proLists = XListProperties (display, win, &nprops);

    printf ("properties num: %d\n", nprops);
    for (int i = 0; i < nprops; ++i) {
        char* name = XGetAtomName (display, proLists[i]);
        printf ("property[%d]=%s\n", i, name);
    }

    XCloseDisplay(display);

    return 0;
}

static Window getFocusedWindow(Display* display)
{
    Window fWindow;
    int revertTo;

    XGetInputFocus(display, &fWindow, &revertTo);

    return fWindow;
}
