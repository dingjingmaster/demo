/*************************************************************************
> FileName: x-all-window-recu.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 16 Aug 2023 05:52:52 PM CST
 ************************************************************************/
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define MAXSTR  500000

static void list_window_property (const Display* display, Window window);

int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;

    display = XOpenDisplay (0);
    rootWindow = DefaultRootWindow (display);

    list_window_property (display, rootWindow);

    XCloseDisplay(display);

    return 0;
}

static void list_window_property (const Display* display, Window window)
{
    Window rootReturn;
    Window parentReturn;

    unsigned int winNum;
    Window* childList = NULL;

    XQueryTree (display, window, &rootReturn, &parentReturn, &childList, &winNum);
    if (!childList || 0 == winNum) {
        printf ("0x%x\n", window);
        return;
    }

    for (int i = 0; i < winNum; ++i) {
        list_window_property (display, childList[i]);
    }

    XFree(childList);
}
