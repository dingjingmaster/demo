/*************************************************************************
> FileName: x-get-forefront-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 23 Aug 2023 01:52:27 PM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

int main (int argc, char* argv[])
{
    Display* dsp = XOpenDisplay(NULL);
    if (NULL == dsp) {
        printf ("打开窗口失败!\n");
        return -1;
    }

    Window focused;
    Window root;
    Window rootRet;
    Window parentRet;
    Window* child;
    unsigned int numChild;

    root = DefaultRootWindow(dsp);

    int revert;
    XGetInputFocus(dsp, &focused, &revert);

    XQueryTree(dsp, root, &rootRet, &parentRet, &child, &numChild);

    // 查找最前面的窗口
    Window topWindow = 0;
    for (unsigned int i = 0; i < numChild; ++i) {
        Window c = child[i];
        if (focused != c) {
            XWindowAttributes attr;
            XGetWindowAttributes(dsp, c, &attr);
            if (attr.map_state == IsViewable && attr.override_redirect == False) {
                topWindow = c;
                printf ("最前面的窗口: %9lu(0x%-9X)\n", topWindow, topWindow);
            }
        }
    }

    XFree(child);
    XCloseDisplay(dsp);

    return 0;
}
