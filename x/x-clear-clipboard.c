/*************************************************************************
> FileName: x-clear-clipboard.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 22 Nov 2023 05:48:41 PM CST
 ************************************************************************/
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <stdio.h>
#include <stdbool.h>

int main (int argc, char* argv[])
{
    Display* dsp = XOpenDisplay(NULL);

    int x11Fd = ConnectionNumber(dsp);
    Window win = XCreateSimpleWindow (dsp, DefaultRootWindow(dsp), 0, 0, 1, 1, 0, 0, 0);
    Atom clip = XA_CLIPBOARD(dsp);
    Atom atom = XInternAtom (dsp, "XCLIP_OUT", false);
    XConvertSelection (dsp, clip, XA_STRING, atom, win, CurrentTime);
    XSetSelectionOwner (dsp, clip, None, CurrentTime);
    XFlush (dsp);

    return 0;
}



