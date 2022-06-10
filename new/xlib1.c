//
// Created by dingjing on 2020/1/14.
//
#include <X11/Xlib.h> /* 每个Xlib必须包含的头文件 */
#include <assert.h>   /* 返回值验证 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(void)
{
    Display *d;
    Window w;
    XEvent e;
    char *msg = "Hello, World!";
    int s;

    bool done = false;

    /* 与XServer 进行连接 */
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    /* 获取屏幕 */
    s = DefaultScreen(d);

    /* 创建窗口 */
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 640, 480, 0,
            BlackPixel(d, s), WhitePixel(d, s));

    /* register interest in the delete window message */
    Atom wmDeleteMessage = XInternAtom(d, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(d, w, &wmDeleteMessage, 1);

    /* 设置感兴趣的事件 */
    XSelectInput(d, w, ExposureMask | KeyPressMask | StructureNotifyMask);

    /* 在`屏幕上`显示`窗口` */
    XMapWindow(d, w);

    /* 事件循环 */
    while (!done) {
        XNextEvent(d, &e);
#if 0
        /* 绘制窗口 */
        if (e.type == Expose) {
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
            XDrawString(d, w, DefaultGC(d, s), 50, 50, msg, strlen(msg));
        }

        /* 按键退出 */
        switch(e.type){
          /*绘制*/
            case KeyPress:
                XDestroyWindow(d, w);
                break;

            case DestroyNotify:
                done = true;
                break;

            case ClientMessage:
                if (e.xclient.data.l[0] == wmDeleteMessage){
                    done = true;
                }
                break;
        }
#endif
    }

    XCloseDisplay(d);

    return 0;
}