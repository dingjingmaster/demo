/*************************************************************************
> FileName: x-grab-key3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 19 Dec 2023 09:52:24 AM CST
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
    XGrabKeyboard(display, root, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    XSelectInput (display, root, KeyPressMask);

    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            key = XLookupKeysym(&event.xkey, 0);
            unsigned int mod = event.xkey.state;
            if (Mod1Mask & mod) {
                printf ("Mod1Mask ");
            }
            if (Mod2Mask & mod) {
                printf ("Mod2Mask ");
            }
            if (Mod3Mask & mod) {
                printf ("Mod3Mask ");
            }
            if (Mod4Mask & mod) {
                printf ("Mod4Mask ");
            }
            if (Mod5Mask & mod) {
                printf ("Mod5Mask ");
            }
            if (ControlMask & mod) {
                printf ("ControlMask ");
            }
            if (LockMask & mod) {
                printf ("LockMask ");
            }
            if (ShiftMask & mod) {
                printf ("ShiftMask ");
            }

            const char* keyStr = XKeysymToString(key);
            printf ("%s(%lu=0x%X)\n", keyStr, key, key);

            if ((mod & ControlMask) && (key == XK_c)) {
                printf("即将退出!\n");
                break;
            }
        }
    }

    XUngrabKeyboard(display, CurrentTime);
    XCloseDisplay (display);

    return 0;
}

