/*************************************************************************
> FileName: x-grab-key4.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 19 Dec 2023 11:47:58 AM CST
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

    printf ("按下 Ctrl+c 退出\n");
    XSelectInput (display, root, KeyPressMask | KeyReleaseMask);

    while (1) {
        XNextEvent(display, &event);
        printf ("%d\n", event.type);
        if (event.type == KeyPress) {
            key = XLookupKeysym(&event.xkey, 0);
            printf ("%u == %u\n", key, XK_Super_L);
            if (XK_Super_L == key) {
                XGrabKeyboard(display, root, True, GrabModeAsync, GrabModeAsync, CurrentTime);
                printf ("grub\n");
                continue;
            }
            unsigned int mod = event.xkey.state;

            const char* keyStr = XKeysymToString(key);
            printf ("%s\n", keyStr);

            if ((mod & ControlMask) && (key == XK_c)) {
                printf("即将退出!\n");
                break;
            }
        }
        else if (event.type == KeyRelease) {
            if (XK_Super_L == key) {
                XUngrabKeyboard(display, CurrentTime);
                continue;
            }
        }
    }

    XUngrabKeyboard(display, CurrentTime);
    XCloseDisplay (display);

    return 0;
}

