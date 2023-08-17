/*************************************************************************
> FileName: x-all-window2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 17 Aug 2023 09:56:48 AM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#define MAXSTR  500000

static void list_window_property (Display* display, Window window);

int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;

    display = XOpenDisplay (NULL);
    rootWindow = DefaultRootWindow (display);

    list_window_property (display, rootWindow);

    XCloseDisplay(display);

    return 0;
}

static void list_window_property (Display* display, Window window)
{
    Window rootReturn;
    Window parentReturn;

    unsigned int winNum;
    Window* childList = NULL;

    XQueryTree (display, window, &rootReturn, &parentReturn, &childList, &winNum);
    for (int i = 0; i < winNum; ++i) {
        Window child = childList[i];
        printf ("ID: %d\t", child);

        // pid
        Atom pidAtom = XInternAtom(display, "_NET_WM_PID", False);
        Atom type;
        int format = 0;
        unsigned long nItems;
        unsigned long bytesAfter;
        unsigned char* propData = NULL;
        int status = XGetWindowProperty (display, child, pidAtom, 0, 1, False, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propData);
        if (Success == status && NULL != propData) {
            if (32 == format && XA_CARDINAL == type && 1 == nItems) {
                unsigned long pid = *((unsigned long*)propData);
                XFree(propData);
                printf ("pid: %lu\t", pid);
            }
        }

        // win name
        char* winName = NULL;
        XFetchName(display, child, &winName);
        if (winName) {
            printf ("Name: %s", winName);
            XFree(winName);
        }


        printf ("\n");
        list_window_property (display, child);
    }
    XFree(childList);
}
