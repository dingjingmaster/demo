/*************************************************************************
> FileName: x-grab-key.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 21 Aug 2023 10:19:00 AM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

int main ()
{
    Display* display = NULL;
    Window root;
    XEvent event;
    KeySym key;

    display = XOpenDisplay(NULL);
    root = DefaultRootWindow (display);

    XGrabKey(display, XKeysymToKeycode(display, XK_x), Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);

    XSelectInput (display, root, KeyPressMask);

    printf ("Press 'x' with Alt key!\n");

    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            key = XLookupKeysym(&event.xkey, 0);
            if (key == XK_x && (event.xkey.state & Mod1Mask)) {
                printf ("Press 'x'\n");
            }
        }
    }

    XCloseDisplay (display);

    return 0;
}
