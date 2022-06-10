/*************************************************************************
> FileName: cairo-demo4.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 16:05:04
 ************************************************************************/
#include <cairo.h>
#include <math.h>

/**
 * 绘制星星
 */

void draw_stars (cairo_t *cr, int width, int height);

void star_path (cairo_t *cr);

#define WIDTH 600
#define HEIGHT 275
#define STRIDE (WIDTH * 4)

unsigned char image[STRIDE*HEIGHT];

int main (void)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create_for_data (image, CAIRO_FORMAT_ARGB32,
						   WIDTH, HEIGHT, STRIDE);

    cr = cairo_create (surface);

    cairo_rectangle (cr, 0, 0, WIDTH, HEIGHT);
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_fill (cr);

    draw_stars (cr, WIDTH, HEIGHT);

    cairo_surface_write_to_png (surface, "stars.png");

    cairo_destroy (cr);

    cairo_surface_destroy (surface);

    return 0;
}

void star_path (cairo_t *cr)
{
    int i;
    double theta = 4 * M_PI / 5.0;

    cairo_move_to (cr, 0, 0);
    for (i=0; i < 4; i++) {
	    cairo_rel_line_to (cr, 1.0, 0);
	    cairo_rotate (cr, theta);
    }
    cairo_close_path (cr);
}

void draw_stars (cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_save (cr);
    {
	    cairo_translate (cr, 5, height / 2.6);
	    cairo_scale (cr, height, height);
	    star_path (cr);
	    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
	    cairo_fill (cr);
    }
    cairo_restore (cr);

    cairo_save (cr);
    {
	    cairo_translate (cr, width - height - 5, height / 2.6);
	    cairo_scale (cr, height, height);
	    star_path (cr);
	    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	    cairo_fill (cr);
    }
    cairo_restore (cr);
}

