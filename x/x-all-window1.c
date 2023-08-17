#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define MAXSTR  500000


int main (int argc, char* argv[])
{
    Display         *display = NULL;

    display = XOpenDisplay (0);

    const int screenNum = ScreenCount(display);
    for (int i = 0; i < screenNum; ++i) {
        Window  dummy;
        int     nChild = 0;
        Window* childWindow = NULL;
        Window rootWindow = RootWindow(display, i);
        Status success = XQueryTree (display, rootWindow, &dummy, &dummy, &childWindow, &nChild);
        if (!success) {
            if (NULL != childWindow) {
                XFree(childWindow);
            }
            break;
        }
        for (int j = 0; j < nChild; ++j) {
            Window child = childWindow[j];
            printf ("%d\n", child);
        }
        
        if (NULL != childWindow) {
            XFree(childWindow);
        }
    }

    XCloseDisplay(display);

    return 0;
}

