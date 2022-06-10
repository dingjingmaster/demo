/*************************************************************************
> FileName: gdk-demo5.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月31日 星期二 16时06分57秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void draw(char *rgb_out, int w, int h)
{
        for(int i=0; i<w*h*4; i++)
                *rgb_out++ = rand();
}

XImage *create_ximage(Display *display, Visual *visual, int width, int height)
{
        char *image32 = (char *)malloc(width * height * 4);
        draw(image32, width, height);
        return XCreateImage(display, visual, 24, ZPixmap, 0, image32, width, height, 32, 0);
}

int x11_err_handler(Display *pd, XErrorEvent *pxev)
{
        char msg[4096] = { 0 };
        XGetErrorText(pd, pxev -> error_code, msg, sizeof(msg));
        printf("%s\n", msg);
        return 0;
}

int main(int argc, char **argv)
{
        Display *display = XOpenDisplay(getenv("DISPLAY"));

        Window window = DefaultRootWindow(display);

        XWindowAttributes wa;
        XGetWindowAttributes(display, window, &wa);
        int width = wa.width, height = wa.height;
        printf("res: %dx%d\n", width, height);

        GC gc = XCreateGC(display, window, 0, NULL);
        Visual *visual = DefaultVisual(display, 0);

        XSetErrorHandler(x11_err_handler);

        for(;;)
        {
                XImage *ximage = create_ximage(display, visual, width, height);
                XPutImage(display, window, gc, ximage, 0, 0, 0, 0, width, height);
                XDestroyImage(ximage);

                XFlush(display);

                usleep(50000);
        }

        return 0;
}

