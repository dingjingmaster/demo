/*************************************************************************
> FileName: cairo-demo7.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 16:40:09
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <cairo.h>
#include <cairo-xlib.h>

#define PI 3.1415926535

typedef struct win {
    Display *dpy;
    int scr;

    Window win;
    GC gc;

    int width, height;
    KeyCode quit_code;
} win_t;

static void triangle(cairo_t *cr);
static void square(cairo_t *cr);
static void bowtie(cairo_t *cr);
static void win_init(win_t *win);
static void win_deinit(win_t *win);
static void win_draw(win_t *win);
static void win_handle_events(win_t *win);

int main(int argc, char *argv[])
{
    win_t win;

    win.dpy = XOpenDisplay(0);
    if (win.dpy == NULL) {
        fprintf(stderr, "Failed to open display\n");
        return 1;
    }

    win_init(&win);
    win_draw(&win);
    win_handle_events(&win);
    win_deinit(&win);
    XCloseDisplay(win.dpy);

    return 0;
}

#define SIZE 20
static void triangle(cairo_t *cr)
{
    cairo_move_to(cr, SIZE, 0);
    cairo_rel_line_to(cr, SIZE,  2*SIZE);
    cairo_rel_line_to(cr, -2*SIZE, 0);
    cairo_close_path(cr);
}

static void square(cairo_t *cr)
{
    cairo_move_to(cr, 0, 0);
    cairo_rel_line_to(cr,  2*SIZE,   0);
    cairo_rel_line_to(cr,   0,  2*SIZE);
    cairo_rel_line_to(cr, -2*SIZE,   0); 
    cairo_close_path(cr);
}

static void bowtie(cairo_t *cr)
{
    cairo_move_to(cr, 0, 0);
    cairo_rel_line_to(cr,  2*SIZE,  2*SIZE);
    cairo_rel_line_to(cr, -2*SIZE,   0); 
    cairo_rel_line_to(cr,  2*SIZE, -2*SIZE);
    cairo_close_path(cr);
}

static void inf(cairo_t *cr)
{
    cairo_move_to(cr, 0, SIZE);
    cairo_rel_curve_to(cr,
        0, SIZE,
        SIZE, SIZE,
        2*SIZE, 0);
    cairo_rel_curve_to(cr,
        SIZE, -SIZE,
        2*SIZE, -SIZE,
        2*SIZE, 0);
    cairo_rel_curve_to(cr,
        0, SIZE,
        -SIZE, SIZE,
        -2*SIZE, 0);
    cairo_rel_curve_to(cr,
        -SIZE, -SIZE,
        -2*SIZE, -SIZE,
        -2*SIZE, 0);
    cairo_close_path(cr);
}

static void draw_shapes(cairo_t *cr, int x, int y, int fill)
{
    cairo_save(cr);

    cairo_new_path(cr);
    cairo_translate(cr, x+SIZE, y+SIZE);
    bowtie(cr);
    if (fill)
        cairo_fill(cr);
    else
        cairo_stroke(cr);

    cairo_new_path(cr);
    cairo_translate(cr, 4*SIZE, 0);
    square(cr);
    if (fill)
        cairo_fill(cr);
    else
        cairo_stroke(cr);

    cairo_new_path(cr);
    cairo_translate(cr, 4*SIZE, 0);
    triangle(cr);
    if (fill)
        cairo_fill(cr);
    else
        cairo_stroke(cr);

    cairo_new_path(cr);
    cairo_translate(cr, 4*SIZE, 0);
    inf(cr);
    if (fill)
        cairo_fill(cr);
    else
        cairo_stroke(cr);
    cairo_restore(cr);
}

static void fill_shapes(cairo_t *cr, int x, int y)
{
    draw_shapes(cr, x, y, 1);
}

static void stroke_shapes(cairo_t *cr, int x, int y)
{
    draw_shapes(cr, x, y, 0);
}

static void win_draw(win_t *win)
{
#define NUM_DASH 2
    static double dash[NUM_DASH] = {SIZE/4.0, SIZE/4.0};
    cairo_surface_t *surface;
    cairo_t *cr;
    Visual *visual = DefaultVisual(win->dpy, DefaultScreen (win->dpy));

    XClearWindow(win->dpy, win->win);

    surface = cairo_xlib_surface_create (win->dpy, win->win, visual,
                win->width, win->height);
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);

    cairo_save(cr);
    cairo_set_font_size (cr, 20);
    cairo_move_to (cr, 10, 10);
    cairo_rotate(cr, PI / 2);
    cairo_show_text (cr, "Hello World.");
    cairo_restore(cr);

    cairo_set_line_width(cr, SIZE / 4);

    cairo_set_tolerance(cr, .1);

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_set_dash(cr, dash, NUM_DASH, 0);
    stroke_shapes(cr, 0, 0);

    cairo_translate(cr, 0, 4*SIZE);

    cairo_set_dash(cr, NULL, 0, 0);
    stroke_shapes(cr, 0, 0);

    cairo_translate(cr, 0, 4*SIZE);

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);
    stroke_shapes(cr, 0, 0);

    cairo_translate(cr, 0, 4*SIZE);

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
    stroke_shapes(cr, 0, 0);

    cairo_translate(cr, 0, 4*SIZE);

    fill_shapes(cr, 0, 0);

    cairo_translate(cr, 0, 4*SIZE);

    cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);
    fill_shapes(cr, 0, 0);
    cairo_set_source_rgb(cr, 1, 0, 0);
    stroke_shapes(cr, 0, 0);
    if (cairo_status (cr)) {
        printf("Cairo is unhappy: %s\n",
        cairo_status_to_string (cairo_status (cr)));
        exit(0);
    }

    cairo_destroy(cr);
    cairo_surface_destroy (surface);
}

static void win_init(win_t *win)
{
    Window root;
    win->width = 400;
    win->height = 400;

    root = DefaultRootWindow(win->dpy);
    win->scr = DefaultScreen(win->dpy);

    win->win = XCreateSimpleWindow(win->dpy, root, 0, 0,
                win->width, win->height, 0,
                BlackPixel(win->dpy, win->scr), BlackPixel(win->dpy, win->scr));

    win->quit_code = XKeysymToKeycode(win->dpy, XStringToKeysym("Q"));

    XSelectInput(win->dpy, win->win,
            KeyPressMask | StructureNotifyMask | ExposureMask);

    XMapWindow(win->dpy, win->win);
}

static void win_deinit(win_t *win)
{
    XDestroyWindow(win->dpy, win->win);
}

static void win_handle_events(win_t *win)
{
    XEvent xev;

    while (1) {
        XNextEvent(win->dpy, &xev);
        switch(xev.type) {
        case KeyPress: {
            XKeyEvent *kev = &xev.xkey;
            if (kev->keycode == win->quit_code)
                return;
        }
        break;
        case ConfigureNotify: {
            XConfigureEvent *cev = &xev.xconfigure;
            win->width = cev->width;
            win->height = cev->height;
        }
        break;
        case Expose: {
            XExposeEvent *eev = &xev.xexpose;

            if (eev->count == 0)
                win_draw(win);
            }
            break;
        }
    }
}

