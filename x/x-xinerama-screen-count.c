/*************************************************************************
> FileName: x-xinerama-screen-count.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 09 Jan 2024 01:20:38 PM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xinerama.h>


int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;

    display = XOpenDisplay (0);
    rootWindow = DefaultRootWindow (display);

    if (XineramaIsActive(display)) {
        int num = 0;
        XineramaScreenInfo* info = XineramaQueryScreens(display, &num);
        printf ("screen num: %d\n", num);
        for (int i = 0; i < num; ++i) {
            printf ("\tscreen number: %d, x: %d, y: %d, width: %d, height: %d\n", 
                    info[i].screen_number,
                    info[i].x_org,
                    info[i].y_org,
                    info[i].width,
                    info[i].height);
        }
    }


    XCloseDisplay(display);

    return 0;
}


