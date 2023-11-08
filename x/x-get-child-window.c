/*************************************************************************
> FileName: x-get-child-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 23 Aug 2023 05:07:13 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Window getFocusedWindow(Display* display);
static void print_child_window(Display* display, Window win);

int main (int argc, char* argv[])
{
    Display* display = XOpenDisplay(NULL);
    Window win = getFocusedWindow(display);
    if (argc == 2) {win = atoi(argv[1]);}
    if (None == win) return -1;

    printf ("focused window ID: %lu(0x%X)\n", win, win);

    print_child_window(display, win);

    XCloseDisplay(display);

    return 0;
}

static Window getFocusedWindow(Display* display)
{
    Window fWindow;
    int revert;
    Window root = DefaultRootWindow(display);

    int status = XGetInputFocus(display, &fWindow, &revert);
    if (None == status) {
        printf ("无效的窗口\n");
        return None;
    }

    Window pT = fWindow;
    Window parentTmp = fWindow;
    Window root1, parent;
    Window* child;
    unsigned int numChild;

    do {
        parent = parentTmp;
        XQueryTree (display, pT, &root1, &parentTmp, &child, &numChild);
        pT = parentTmp;
        XFree (child);
    } while (parentTmp != root);

    printf ("window ID: 0x%X, parent window ID: 0x%X, root1: 0x%X\n", fWindow, parent, root1);

    return parent;
}

static void print_child_window(Display* display, Window win)
{
    Window pT = win;
    Window parentTmp = pT;
    Window root1, parent;
    Window* child;
    unsigned int numChild;
    XClassHint classHint;

    static int idx = 0;

    for (int j = 0; j < idx * 4; ++j) printf(" ");
    printf ("0x%X", win); // classHint.res_class ? classHint.res_class : "");
    if (0 != XGetClassHint(display, win, &classHint)) {
        printf (" %s(%s)", classHint.res_class?classHint.res_class:"", classHint.res_name?classHint.res_name:"");
    }
    printf ("\n");

    XQueryTree (display, pT, &root1, &parentTmp, &child, &numChild);
    for (int i = 0; i < numChild; ++i) {
        ++idx;
        print_child_window(display, child[i]);
        --idx;
    }
}

