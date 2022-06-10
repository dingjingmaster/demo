/*************************************************************************
> FileName: vte-all-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年04月03日 星期五 08时13分52秒
 ************************************************************************/
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;
    unsigned int    winNum;
    Window          rootReturn, parentReturn;
    Window*         childList = NULL;

    display = XOpenDisplay (0);
    rootWindow = DefaultRootWindow (display);

    XQueryTree (display, rootWindow, &rootReturn, &parentReturn, &childList, &winNum);

    printf ("%d\n", winNum);


    XFree(childList);
    XCloseDisplay(display);

    return 0;
}
