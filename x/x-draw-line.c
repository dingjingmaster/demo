/*************************************************************************
> FileName: x-draw-line.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年05月14日 星期五 08时17分13秒
 ************************************************************************/
#include <assert.h>
#include <unistd.h>
#include <X11/Xlib.h>


int main (int argc, char* argv[])
{
    Display* dpy = XOpenDisplay (0);
    assert (dpy);

    int blackColor = BlackPixel (dpy, DefaultScreen(dpy));
    int whiteColor = WhitePixel (dpy, DefaultScreen(dpy));

    // creat the window
    Window w = XCreateSimpleWindow (dpy, DefaultRootWindow(dpy), 0, 0, 200, 100, 0, blackColor, blackColor);

    // want to get MapNotify events
    XSelectInput (dpy, w, StructureNotifyMask);

    // Map the window
    XMapWindow (dpy, w);

    // create a Graphics Context
    GC gc = XCreateGC (dpy, w, 0, 0);

    // tell the GC we draw using the white color
    XSetForeground (dpy, gc, whiteColor);

    // wait for the mapnotify event
    for (;;) {
        XEvent e;
        XNextEvent (dpy, &e);
        if (e.type == MapNotify) {
            break;
        }
    }

    // Draw the line
    XDrawLine (dpy, w, gc, 10, 60, 180, 20);

    // send the Draw Line request to the server
    XFlush (dpy);

    sleep (10);
}
