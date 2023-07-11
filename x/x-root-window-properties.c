/*************************************************************************
> FileName: x-root-window-properties.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年11月07日 星期日 13时20分54秒
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

    int nprops = 0;
    Atom* proLists = XListProperties (display, rootWindow, &nprops);

    printf ("properties num: %d\n", nprops);

    for (int i = 0; i < nprops; ++i) {
        char* name = XGetAtomName (display, proLists[i]);
        printf ("property[%d]=%s\n", i, name);
    }


    XFree(childList);
    XCloseDisplay(display);

    return 0;
}
