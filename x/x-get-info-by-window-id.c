/*************************************************************************
> FileName: x-get-info-by-window-id.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 22 Aug 2023 09:04:46 AM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static void print_window_property (Display* display, Window window);

int main (int argc, char* argv[])
{
    Display         *display = NULL;
    Window          rootWindow;

    display = XOpenDisplay (0);
    rootWindow = DefaultRootWindow (display);

    //
    print_window_property (display, rootWindow);

    XCloseDisplay(display);

    return 0;
}

static void print_window_property (Display* display, Window window)
{
    if (!display) return;

    //
    XWindowAttributes attrs;
    Status status = XGetWindowAttributes(display, window, &attrs);
    if (0 == status) {
        printf ("无法获取\n");
        return;
    }

    printf ("窗口位置   : (%d, %d)\n", attrs.x, attrs.y);
    printf ("窗口大小   : (%d, %d)\n", attrs.width, attrs.height);
    printf ("边框宽度   : %d\n", attrs.border_width);
    printf ("父窗口     : %d\n", attrs.root);
    printf ("     : %d\n", attrs.root);

#if 0
    typedef struct {
        int x, y;       /* location of window */
        int width, height;      /* width and height of window */
        int border_width;       /* border width of window */
        int depth;      /* depth of window */
        Visual *visual; /* the associated visual structure */
        Window root;    /* root of screen containing window */
        int class;      /* InputOutput, InputOnly*/
        int bit_gravity;        /* one of the bit gravity values */
        int win_gravity;        /* one of the window gravity values */
        int backing_store;      /* NotUseful, WhenMapped, Always */
        unsigned long backing_planes;   /* planes to be preserved if possible */
        unsigned long backing_pixel;    /* value to be used when restoring planes */
        Bool save_under;        /* boolean, should bits under be saved? */
        Colormap colormap;      /* color map to be associated with window */
        Bool map_installed;     /* boolean, is color map currently installed*/
        int map_state;  /* IsUnmapped, IsUnviewable, IsViewable */
        long all_event_masks;   /* set of events all people have interest in*/
        long your_event_mask;   /* my event mask */
        long do_not_propagate_mask;     /* set of events that should not propagate */
        Bool override_redirect; /* boolean value for override-redirect */
        Screen *screen; /* back pointer to correct screen */
    } XWindowAttributes;

#endif

    return;
}
