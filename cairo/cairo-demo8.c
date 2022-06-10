/*************************************************************************
> FileName: cairo-demo8.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 18:17:14
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <cairo.h>
#include <cairo-xlib.h>

#define EPSILON (1.0 / (2<<16))
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

typedef struct color {
    double red;
    double green;
    double blue;
} color_t;

typedef struct pt {
    double x;
    double y;
} pt_t;

typedef struct spline {
    pt_t pt[4];
} spline_t;

typedef struct quadr {
    pt_t pt[4];
} quadr_t;

typedef struct win {
    Display *dpy;
    int scr;
    Window win;
    GC gc;
    Pixmap pix;
    int width, height;
    long event_mask;

    int needs_refresh;

    double tolerance;
    double line_width;
    cairo_line_cap_t line_cap;
    spline_t spline;
    double zoom;
    double xtrans;
    double ytrans;

    int click;
    pt_t drag_pt;
    int active;
} win_t;

typedef struct callback_doc {
    void *callback;
    char *doc;
} callback_doc_t;

typedef int (*key_callback_t)(win_t *win);

typedef struct key_binding
{
    char *key;
    int is_alias;
    KeyCode keycode;
    key_callback_t callback;
} key_binding_t;

static void win_init(win_t *win, Display *dpy);
static void win_deinit(win_t *win);
static void win_refresh(win_t *win);
static void win_select_events(win_t *win);
static void win_handle_events(win_t *win);
static void win_print_help(win_t *win);

static int quit_cb(win_t *win);
static int print_spline_cb(win_t *win);
static int zoom_in_cb(win_t *win);
static int zoom_out_cb(win_t *win);
static int trans_left_cb(win_t *win);
static int trans_right_cb(win_t *win);
static int trans_up_cb(win_t *win);
static int trans_down_cb(win_t *win);
static int flatten_cb(win_t *win);
static int smooth_cb(win_t *win);
static int widen_line(win_t *win);
static int narrow_line(win_t *win);

static const double DEFAULT_XTRANS = 0.0;
static const double DEFAULT_YTRANS = 0.0;
static const double DEFAULT_TOLERANCE = .1;
static const double DEFAULT_LINE_WIDTH = 10;
static const double DEFAULT_ZOOM = 1.0;
static const cairo_line_cap_t DEFAULT_LINE_CAP = CAIRO_LINE_CAP_ROUND;

static const spline_t ribbon = {
    { {110, 20}, {310, 300}, {10, 310}, {210, 20} }
};

#define DEFAULT_SPLINE ribbon

static const callback_doc_t callback_doc[] = {
    { quit_cb,		"Exit the program" },
    { print_spline_cb,	"Print current spline coordinates on stdout" },
    { zoom_in_cb,	"Zoom in" },
    { zoom_out_cb,	"Zoom out" },
    { trans_left_cb,	"Translate left" },
    { trans_right_cb,	"Translate right" },
    { trans_up_cb,	"Translate up" },
    { trans_down_cb,	"Translate down" },
    { flatten_cb,	"Decrease rendering accuracy, (tolerance *= 10)" },
    { smooth_cb,	"Increase rendering accuracy, (tolerance /= 10)" },
    { widen_line,	"Widen line width" },
    { narrow_line,	"Narrow line width" },
};

static key_binding_t key_binding[] = {
    { "Q",	0, 0, quit_cb },
    { "Left",	0, 0, trans_left_cb },
    { "Right",	0, 0, trans_right_cb },
    { "Up",	0, 0, trans_up_cb },
    { "Down",	0, 0, trans_down_cb },
    { "Return",	0, 0, print_spline_cb },
    { "plus",	0, 0, zoom_in_cb },
    { "equal",	1, 0, zoom_in_cb },
    { "minus",	0, 0, zoom_out_cb },
    { "greater",0, 0, smooth_cb },
    { "period",	1, 0, smooth_cb },
    { "less",	0, 0, flatten_cb },
    { "comma",	1, 0, flatten_cb },
    { "W",	0, 0, widen_line },
    { "N",	0, 0, narrow_line },
};

int main(int argc, char *argv[])
{
    win_t win;

    Display *dpy = XOpenDisplay(0);

    if (dpy == NULL) {
        fprintf(stderr, "Failed to open display: %s\n", XDisplayName(0));
        return 1;
    }

    win_init(&win, dpy);

    win_print_help(&win);
    win_handle_events(&win);
    win_deinit(&win);

    XCloseDisplay(dpy);

    return 0;
}

static void draw_control_line(cairo_t *cr, pt_t *a, pt_t *b, double width)
{
    cairo_save(cr);

    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr, width);

    cairo_move_to(cr, a->x, a->y);
    cairo_line_to(cr, b->x, b->y);
    cairo_stroke(cr);

    cairo_restore(cr);
}

static void draw_spline(cairo_t *cr, win_t *win)
{
    spline_t *spline = &win->spline;
    double zoom = win->zoom;
    double drag_user_x = win->drag_pt.x;
    double drag_user_y = win->drag_pt.y;

    int i;

    cairo_device_to_user (cr, &drag_user_x, &drag_user_y);

    cairo_save(cr);

    cairo_move_to(cr, spline->pt[0].x, spline->pt[0].y);
    cairo_curve_to(cr,
	      spline->pt[1].x, spline->pt[1].y,
	      spline->pt[2].x, spline->pt[2].y,
	      spline->pt[3].x, spline->pt[3].y);

    if (win->click && cairo_in_stroke (cr, drag_user_x, drag_user_y)) {
        win->active = 0xf;
    }

    cairo_stroke(cr);

    draw_control_line(cr, &spline->pt[0], &spline->pt[1], 2.0 / zoom);
    draw_control_line(cr, &spline->pt[3], &spline->pt[2], 2.0 / zoom);

    for (i=0; i < 4; i++) {
        cairo_save(cr);
        cairo_set_source_rgba (cr, 1, 0, 0, 0.5);

        cairo_new_path (cr);
        cairo_arc (cr,
                spline->pt[i].x, spline->pt[i].y,
                win->line_width / 1.25, 0, 2 * M_PI);
        if (win->click && cairo_in_fill (cr, drag_user_x, drag_user_y)) {
            win->active = (1<<i);
            win->click = 0;
        }
        cairo_fill (cr);
        cairo_restore(cr);
    }

    cairo_restore(cr);
}

static void win_refresh(win_t *win)
{
    Display *dpy = win->dpy;

    cairo_surface_t *surface;
    cairo_t *cr;
    cairo_status_t status;

    XFillRectangle(dpy, win->pix, win->gc, 0, 0, win->width, win->height);

    surface = cairo_xlib_surface_create (dpy, win->pix,
            DefaultVisual (dpy, win->scr),
            win->width, win->height);
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 0, 0, 0);

    cairo_set_line_width(cr, win->line_width);
    cairo_set_line_cap(cr, win->line_cap);
    cairo_translate(cr, win->xtrans, win->ytrans);
    cairo_scale(cr, win->zoom, win->zoom);
    cairo_set_tolerance(cr, win->tolerance);

    draw_spline(cr, win);

    status = cairo_status(cr);
    if (status) {
        fprintf(stderr, "Cairo is unhappy: %s\n",
        cairo_status_to_string(status));
    }

    cairo_destroy(cr);
    cairo_surface_destroy (surface);

    XCopyArea(win->dpy, win->pix, win->win, win->gc,
            0, 0, win->width, win->height, 0, 0);
}

static void win_init(win_t *win, Display *dpy)
{
    int i;
    Window root;
    XGCValues gcv;

    win->dpy = dpy;
    win->width = 400;
    win->height = 400;

    root = DefaultRootWindow(dpy);
    win->scr = DefaultScreen(dpy);

    win->win = XCreateSimpleWindow(dpy, root, 0, 0,
                win->width, win->height, 0,
                WhitePixel(dpy, win->scr), WhitePixel(dpy, win->scr));

    win->pix = XCreatePixmap(dpy, win->win, win->width, win->height, DefaultDepth (dpy, win->scr));
    gcv.foreground = WhitePixel(dpy, win->scr);
    win->gc = XCreateGC(dpy, win->pix, GCForeground, &gcv);
    XFillRectangle(dpy, win->pix, win->gc, 0, 0, win->width, win->height);

    for (i=0; i < ARRAY_SIZE(key_binding); i++) {
        KeySym keysym;
        keysym = XStringToKeysym(key_binding[i].key);
        if (keysym == NoSymbol)
            fprintf(stderr, "ERROR: No keysym for \"%s\"\n", key_binding[i].key);
        else
            key_binding[i].keycode = XKeysymToKeycode(dpy, keysym);
    }

    win->active = 0;
    win->spline = DEFAULT_SPLINE;
    win->tolerance = DEFAULT_TOLERANCE;
    win->line_width = DEFAULT_LINE_WIDTH;
    win->line_cap = DEFAULT_LINE_CAP;
    win->zoom = DEFAULT_ZOOM;
    win->xtrans = DEFAULT_XTRANS;
    win->ytrans = DEFAULT_YTRANS;

    win->click = 0;
    win->drag_pt.x = 0.0;
    win->drag_pt.y = 0.0;

    win_refresh(win);
    win->needs_refresh = 0;

    win_select_events(win);

    XMapWindow(dpy, win->win);
}

static void win_deinit(win_t *win)
{
    XFreeGC(win->dpy, win->gc);
    XFreePixmap(win->dpy, win->pix);
    XDestroyWindow(win->dpy, win->win);
}

static void win_select_events(win_t *win)
{
    win->event_mask = ButtonPressMask
                | ButtonReleaseMask
                | PointerMotionMask
                | KeyPressMask
                | StructureNotifyMask
                | ExposureMask;
    XSelectInput(win->dpy, win->win, win->event_mask);
}

static char* get_callback_doc(void *callback)
{
    int i;

    for (i=0; i < ARRAY_SIZE(callback_doc); i++)
    if (callback_doc[i].callback == callback)
        return callback_doc[i].doc;
    return "<undocumented function>";
}

static void win_print_help(win_t *win)
{
    int i;

    printf("A cairo spline demonstration\n");
    printf("Click and drag to move the spline or adjust its controls. Or:\n\n");

    for (i=0; i < ARRAY_SIZE(key_binding); i++)
        if (! key_binding[i].is_alias)
            printf("%s:\t%s\n", key_binding[i].key,
                    get_callback_doc(key_binding[i].callback));
}

static void win_handle_button_press(win_t *win, XButtonEvent *bev)
{
    win->click = 1;
    win->drag_pt.x = bev->x;
    win->drag_pt.y = bev->y;
 
    win->needs_refresh = 1;
}

static void win_handle_motion(win_t *win, XMotionEvent *mev)
{
    int i;

    if (win->active == 0)
        return;

    for (i = 0; i < 4; i++) {
        if (((1<<i) & win->active) == 0)
            continue;
        win->spline.pt[i].x += (mev->x - win->drag_pt.x) / win->zoom;
        win->spline.pt[i].y += (mev->y - win->drag_pt.y) / win->zoom;
        win->needs_refresh = 1;
    }

    win->drag_pt.x = mev->x;
    win->drag_pt.y = mev->y;
}

static int win_handle_key_press(win_t *win, XKeyEvent *kev)
{
    int i;

    for (i=0; i < ARRAY_SIZE(key_binding); i++)
        if (key_binding[i].keycode == kev->keycode)
            return (key_binding[i].callback)(win);

    return 0;
}

static void win_grow_pixmap(win_t *win)
{
    Pixmap new;

    new = XCreatePixmap(win->dpy, win->win, win->width, win->height, DefaultDepth (win->dpy, win->scr));
    XFillRectangle(win->dpy, new, win->gc, 0, 0, win->width, win->height);
    XCopyArea(win->dpy, win->pix, new, win->gc, 0, 0, win->width, win->height, 0, 0);
    XFreePixmap(win->dpy, win->pix);
    win->pix = new;
    win->needs_refresh = 1;
}

static void win_handle_configure(win_t *win, XConfigureEvent *cev)
{
    int has_grown = 0;

    if (cev->width > win->width || cev->height > win->height) {
        has_grown = 1;
    }

    win->width = cev->width;
    win->height = cev->height;

    if (has_grown) {
        win_grow_pixmap(win);
    }
}

static void win_handle_expose(win_t *win, XExposeEvent *eev)
{
    XCopyArea(win->dpy, win->pix, win->win, win->gc,
        eev->x, eev->y, eev->width, eev->height, eev->x, eev->y);
}

static void win_handle_events(win_t *win)
{
    int done;
    XEvent xev;

    while (1) {
        if (!XPending(win->dpy) && win->needs_refresh) {
            win_refresh(win);
            win->needs_refresh = 0;
        }
        XNextEvent(win->dpy, &xev);
        switch(xev.type) {
        case ButtonPress:
            win_handle_button_press(win, &xev.xbutton);
            break;
        case MotionNotify:
            win_handle_motion(win, &xev.xmotion);
            break;
        case ButtonRelease:
            win->click = 0;
            win->active = 0;
            break;
        case KeyPress:
            done = win_handle_key_press(win, &xev.xkey);
            if (done)
                return;
            break;
        case ConfigureNotify:
            win_handle_configure(win, &xev.xconfigure);
            break;
        case Expose:
            win_handle_expose(win, &xev.xexpose);
            break;
        }
    }
}

static int quit_cb(win_t *win)
{
    return 1;
}

static int print_spline_cb(win_t *win)
{
    pt_t *pt = win->spline.pt;

    printf("{ { %.20g, %.20g }, { %.20g, %.20g }, { %.20g, %.20g }, { %.20g, %.20g } }\n",
        pt[0].x, pt[0].y,
        pt[1].x, pt[1].y,
        pt[2].x, pt[2].y,
        pt[3].x, pt[3].y);

    return 0;
}

static int zoom_in_cb(win_t *win)
{
    win->zoom *= 1.1;
    win->needs_refresh = 1;

    return 0;
}

static int trans_left_cb(win_t *win)
{
    win->xtrans -= win->width / 16.0;
    win->needs_refresh = 1;

    return 0;
}

static int trans_right_cb(win_t *win)
{
    win->xtrans += win->width / 16.0;
    win->needs_refresh = 1;

    return 0;
}

static int trans_up_cb(win_t *win)
{
    win->ytrans -= win->height / 16.0;
    win->needs_refresh = 1;

    return 0;
}

static int trans_down_cb(win_t *win)
{
    win->ytrans += win->height / 16.0;
    win->needs_refresh = 1;

    return 0;
}

static int zoom_out_cb(win_t *win)
{
    win->zoom /= 1.1;
    win->needs_refresh = 1;

    return 0;
}

static int flatten_cb(win_t *win)
{
    win->tolerance *= 10;
    win->needs_refresh = 1;

    return 0;
}

static int smooth_cb(win_t *win)
{
    win->tolerance /= 10;
    win->needs_refresh = 1;

    return 0;
}

static int widen_line(win_t *win)
{
    win->line_width *= 2;
    win->needs_refresh = 1;

    return 0;
}

static int narrow_line(win_t *win)
{
    win->line_width /= 2;
    win->needs_refresh = 1;

    return 0;
}

