/*************************************************************************
> FileName: x-key.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 14 Dec 2023 09:40:08 AM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

Display* display;

void on_hot_key()
{

}

int main (void)
{
    display = XOpenDisplay(NULL);
    if (NULL == display) {
        fprintf (stderr, "XOpenDisplay\n");
        return -1;
    }

    // 
    Window root = DefaultRootWindow(display);
    Window win = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, 0, 0);

    XGrabKey(display, XKeysymToKeycode(display, XK_A), Mod1Mask | ShiftMask, root, True, GrabModeAsync, GrabModeAsync);

    XSelectInput(display, win, KeyPressMask);

    XEvent event;
    
    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            printf ("key code: %u, state: %u", event.xkey.keycode, event.xkey.state);
            // 检查按键是否匹配快捷键
            if (event.xkey.keycode == XKeysymToKeycode (display, XK_A) 
                    && (event.xkey.state & Mod1Mask) && (event.xkey.state & ShiftMask)) {
                printf ("hotKeyPress!\n");
            }
        }
    }

    XCloseDisplay(display);

    return 0;
}
