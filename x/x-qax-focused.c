/*************************************************************************
> FileName: x-qax-focused.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 23 Aug 2023 02:18:53 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

int main (int argc, char* argv[])
{
    Display* dsp = XOpenDisplay (NULL);
    Window win = DefaultRootWindow (dsp);

    Window rootReturn;
    Window parentReturn;

    unsigned int winNum;
    Window* child = NULL;

    Atom qaxF = XInternAtom (dsp, "QAX_WINDOW_WEBVIEW_FOCUS", false);
    Atom pidA = XInternAtom (dsp, "_NET_WM_PID", false);
    XQueryTree (dsp, win, &rootReturn, &parentReturn, &child, &winNum);
    for (int i = 0; i < winNum; ++i) {
        Window c = child[i];
        Atom aType;
        long nItems;
        int aFormat;
        long bytesAfter;
        char* value;
        int pid;
        int status = XGetWindowProperty(dsp, c, pidA, 0, 1, false, AnyPropertyType, &aType, &aFormat, &nItems, &bytesAfter, (unsigned char**)&value);
        if ((Success == status) && value) {
            pid = (*(unsigned long*)value);
            XFree(value);
        }
        printf ("0x%8X(%8u)  %6d\n", c, c, pid);
        status = XGetWindowProperty(dsp, c, qaxF, 0, 1, false, AnyPropertyType, &aType, &aFormat, &nItems, &bytesAfter, (unsigned char**)&value);
        if ((Success == status) && value) {
            int val = (*(int*)value);
            printf ("focused: %s", (val==0) ? "true" : "false");
            break;
        }
    }

    XCloseDisplay(dsp);

    return 0;
}

