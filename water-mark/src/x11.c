/*************************************************************************
> FileName: x11.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 09 Jun 2023 09:51:16 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xinerama.h>

#include <cairo.h>
#include <cairo-xlib.h>

static void draw_text (cairo_t* const cr, int xshapeMask);
static bool compositor_check (Display* d, int screen);

int x11_backend_start ()
{
    Display* dis = XOpenDisplay (NULL);
    Window root = DefaultRootWindow (dis);
    int defaultScreen = XDefaultScreen (dis);

    // 检查窗口管理器
    bool compositorIsRunning = compositor_check (dis, XDefaultScreen(dis));
    if (!compositorIsRunning) {
        printf ("窗口管理器未运行\n");
    }

    int numEntries = 0;
    XineramaScreenInfo* si = XineramaQueryScreens (dis, &numEntries);
    if (NULL == si) {
        printf ("获取屏幕数量信息错误\n");
        XCloseDisplay(dis);
        return 1;
    }

    int xerrBase;
    int xerrEventBase;
    if (!XRRQueryExtension (dis, &xerrEventBase, &xerrBase)) {
        XFree(si);
        XCloseDisplay(dis);
        printf ("获取X扩展失败!\n");
        return 1;
    }

    XRRSelectInput(dis, root, RRScreenChangeNotifyMask);

    XSetWindowAttributes attrs;
    attrs.override_redirect = 1;

    XVisualInfo vinfo;
    int colorDepth = 32;

    if (!XMatchVisualInfo(dis, defaultScreen, colorDepth, TrueColor, &vinfo)) {
        printf ("");
        exit (-1);
    }

    attrs.colormap = XCreateColormap(dis, root, vinfo.visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;

    Window overlay[numEntries];
    cairo_surface_t* surface[numEntries];
    cairo_t* cairoCtx[numEntries];

    Pixmap xshapeMask[numEntries];
    cairo_surface_t* xshapeSurface[numEntries];
    cairo_t* xshapeCtx[numEntries];

    int ovHeight = 200;
    int ovWidth = 500;

    for (int i = 0; i < numEntries; ++i) {
        overlay[i] = XCreateWindow(
                dis,
                root,
                si[i].x_org + si[i].width - ovWidth,
                si[i].y_org + si[i].height - ovHeight,
                ovWidth,
                ovHeight,
                0,
                vinfo.depth,
                InputOutput,
                vinfo.visual,
                CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel,
                &attrs
                );

        XSelectInput(dis, overlay[i], ExposureMask);
        XMapWindow(dis, overlay[i]);

        // 允许鼠标穿越水印
        XRectangle rect;
        XserverRegion region = XFixesCreateRegion(dis, &rect, 1);
        XFixesSetWindowShapeRegion (dis, overlay[i], ShapeInput, 0, 0, region);
        XFixesDestroyRegion(dis, region);

        // 设置
        XClassHint* xch = XAllocClassHint();
        xch->res_name = "water-mark";
        xch->res_class = "water-mark";
        XSetClassHint(dis, overlay[i], xch);

        unsigned char data = 1;
        XChangeProperty (dis, overlay[i], XInternAtom(dis, "_NET_WM_BYPASS_COMPOSITOR", False), XA_CARDINAL, 32, PropModeReplace, &data, 1);
        XChangeProperty (dis, overlay[i], XInternAtom(dis, "GAMESCOPE_EXTERNAL_OVERLAY", False), XA_CARDINAL, 32, PropModeReplace, &data, 1);

        surface[i] = cairo_xlib_surface_create(dis, overlay[i], vinfo.visual, ovWidth, ovHeight);
        cairoCtx[i] = cairo_create(surface[i]);
    }

    XEvent event;
    while (1) {
        XNextEvent(dis, &event);
        // handle screen resize via catching Xrandr event
        if (XRRUpdateConfiguration(&event)) {
            if (event.type - xerrEventBase == RRScreenChangeNotify) {
                si = XineramaQueryScreens(dis, &numEntries);
                for (int i = 0; i < numEntries; i++) {
                    XMoveWindow(dis,                                                                // display
                                overlay[i],                                                         // window
                                si[i].x_org + si[i].width + ovWidth,                                // x position
                                si[i].y_org + si[i].height + ovHeight                               // y position
                    );
                }
            }
        } 
        else if (Expose == event.type) {
            for (int i = 0; i < numEntries && event.xexpose.count == 0; ++i) {
                if (overlay[i] == event.xexpose.window) {
                    draw_text (cairoCtx[i], 0);
                    break;
                }
            }
        }
    }

    // free resources
    for (int i = 0; i < numEntries; ++i) {
        XUnmapWindow (dis, overlay[i]);
        cairo_destroy (cairoCtx[i]);
        cairo_surface_destroy (surface[i]);
    }

    XFree(si);
    XCloseDisplay(dis);

    return 0;
}

int x11_backend_kill_running (void)
{
    return 1;
}

static bool compositor_check(Display* d, int screen)
{
    char propName[16];
    snprintf (propName, 16, "_NET_WM_CM_S%d", screen);
    Atom propAtom = XInternAtom (d, propName, False);

    return XGetSelectionOwner (d, propAtom) != None;
}

static void draw_text (cairo_t* const cr, int xshapeMask)
{
    // clear surface
    cairo_operator_t prevOp = cairo_get_operator(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);

    if (0 == xshapeMask) {
        cairo_set_operator (cr, prevOp);
    }

    // set text color
    cairo_set_source_rgba(cr, 128, 128, 128, 1);

    cairo_font_options_t *font_options = cairo_font_options_create();

    cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_GRAY);
    //cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_NONE);
    cairo_set_font_options(cr, font_options);
    
    cairo_set_font_size (cr, 24);

    cairo_font_weight_t font_weight = CAIRO_FONT_WEIGHT_NORMAL;
    // font_weight = CAIRO_FONT_WEIGHT_BOLD;
    
    cairo_font_slant_t font_slant = CAIRO_FONT_SLANT_NORMAL;
    // font_slant = CAIRO_FONT_SLANT_ITALIC;

    cairo_select_font_face(cr, "", font_slant, font_weight);

    cairo_move_to(cr, 20, 30);
    cairo_show_text(cr, "title");

    cairo_set_font_size(cr, 16);
    cairo_move_to(cr, 20, 55);

    //char *subtitle = options.subtitle;
    //char *new_line_ptr = strchr(subtitle, '\n');
    //cairo_show_text(cr, subtitle);

    cairo_font_options_destroy(font_options);
}
