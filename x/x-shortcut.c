/*************************************************************************
> FileName: x-shortcut.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月23日 星期三 09时04分42秒
 ************************************************************************/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>

int main()
{
    Display*    dpy     = XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    XEvent      ev;

    unsigned int    modifiers       = ControlMask | ShiftMask;
    int             keycode         = XKeysymToKeycode(dpy,XK_Y);
    Window          grab_window     =  root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    XGrabKey(dpy, keycode, modifiers, grab_window, owner_events, pointer_mode,
             keyboard_mode);

    XSelectInput(dpy, root, KeyPressMask );
    while(true)
    {
        int shouldQuit = 0;
        XNextEvent(dpy, &ev);
        switch(ev.type)
        {
            case KeyPress:
                printf("Hot key pressed!");
                XUngrabKey(dpy,keycode,modifiers,grab_window);
                shouldQuit = 1;

            default:
                break;
        }

        if(shouldQuit)
            break;
    }

    XCloseDisplay(dpy);
    return 0;
}
