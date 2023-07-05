/*************************************************************************
> FileName: x-drag-and-drop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 05 Jul 2023 08:58:51 AM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#define DND_VERSION     2
#define DND_ENTER       1
#define DND_LEAVE       2
#define DND_MOTION      3
#define DND_STATUS      4
#define DND_DROP        5
#define DND_FINISHED    6

int main (int argc, char* argv[])
{
    Display* disp = XOpenDisplay(NULL);
    Window window = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 0, 0, 100, 100, 0, 0, 0);
    Atom dndType = XInternAtom (disp, "DND_SELECTION", False);
    Atom dndDrop = XInternAtom (disp, "DND_DRAG_AND_DROP", False);

    XSelectInput(disp, window, ButtonPressMask);
    XMapWindow(disp, window);

    XEvent event;

    while (1) {
        XNextEvent(disp, &event);
        if (ButtonPress == event.type) {
            // START
            XEvent dndEvent;
            dndEvent.type = ClientMessage;
            dndEvent.xclient.window = window;
            dndEvent.xclient.message_type = dndType;
            dndEvent.xclient.format = 32;
            dndEvent.xclient.data.l[0] = DND_ENTER;
            dndEvent.xclient.data.l[1] = dndDrop;
            dndEvent.xclient.data.l[2] = DND_VERSION;
            dndEvent.xclient.data.l[3] = Button1Mask;
            dndEvent.xclient.data.l[4] = 0;

            XSendEvent(disp, window, False, NoEventMask, &dndEvent);

            XGrabPointer(disp, window, False, Button1MotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
        }
        else if (MotionNotify == event.type) {
            // Track mouse
            XEvent dndEvent;
            dndEvent.type = ClientMessage;
            dndEvent.xclient.window = window;
            dndEvent.xclient.message_type = dndType;
            dndEvent.xclient.format = 32;
            dndEvent.xclient.data.l[0] = DND_MOTION;
            dndEvent.xclient.data.l[1] = dndDrop;
            dndEvent.xclient.data.l[2] = Button1Mask;
            dndEvent.xclient.data.l[3] = 0;

            XSendEvent(disp, window, False, NoEventMask, &dndEvent);
        }
        else if (ButtonRelease == event.type) {
            XEvent dndEvent;
            dndEvent.type = ClientMessage;
            dndEvent.xclient.window = window;
            dndEvent.xclient.message_type = dndType;
            dndEvent.xclient.format = 32;
            dndEvent.xclient.data.l[0] = DND_DROP;
            dndEvent.xclient.data.l[1] = dndDrop;
            dndEvent.xclient.data.l[2] = Button1Mask;
            dndEvent.xclient.data.l[3] = 0;
            dndEvent.xclient.data.l[4] = (long) 0;

            XSendEvent(disp, window, False, NoEventMask, &dndEvent);
            XUngrabPointer(disp, CurrentTime);
        }
        else if (ClientMessage == event.type
                    && dndType == event.xclient.message_type
                    && DND_FINISHED == event.xclient.data.l[0]) {
        }
    }

    XCloseDisplay(disp);

    return 0;
}
