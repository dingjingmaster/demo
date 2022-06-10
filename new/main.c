#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#define ATOM (XInternAtom(display, #a, False))

int main (int argc, char* argv[])
{
    Display* display = NULL;
    unsigned char* prop = NULL;
    static int set_transparent = 0;

    display = XOpenDisplay(NULL);

    // X11_create_window
    // init_window (x11.c)
    prop = ATOM(_NET_WM_WINDOW_TYPE_DESKTOP);
    XCreateWindow(display, D, 600, 600, 800, 800, 0, depth, inputo, visual, flags, &attrs);


    return 0;
}