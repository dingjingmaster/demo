/*************************************************************************
> FileName: cairo-demo6.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Вс 22 дек 2019 16:34:32
 ************************************************************************/

#include <cairo.h>
#include <math.h>

#define WIDTH 600
#define HEIGHT 600
#define STRIDE (WIDTH * 4)

unsigned char image[STRIDE*HEIGHT];

static void ring_path (cairo_t *cr)
{
    cairo_move_to (cr, 200.86568, 667.80795);
    cairo_curve_to (cr,
        110.32266, 562.62134,
        122.22863, 403.77940,
        227.41524, 313.23637);
    cairo_curve_to (cr,
        332.60185, 222.69334,
        491.42341, 234.57563,
        581.96644, 339.76224);
    cairo_curve_to (cr,
        672.50948, 444.94884,
        660.64756, 603.79410,
        555.46095, 694.33712);
    cairo_curve_to (cr,
        450.27436, 784.88016,
        291.40871, 772.99456,
        200.86568, 667.80795);
    cairo_close_path (cr);

    cairo_move_to (cr, 272.14411, 365.19927);
    cairo_curve_to (cr,
        195.64476, 431.04875,
        186.97911, 546.57972,
        252.82859, 623.07908);
    cairo_curve_to (cr,
        318.67807, 699.57844,
        434.23272, 708.22370,
        510.73208, 642.37422);
    cairo_curve_to (cr,
        587.23144, 576.52474,
        595.85301, 460.99047,
        530.00354, 384.49112);
    cairo_curve_to (cr,
        464.15406, 307.99176,
        348.64347, 299.34979,
        272.14411, 365.19927);
    cairo_close_path (cr);
}

static void star_path (cairo_t *cr)
{
    cairo_matrix_t matrix;

    cairo_matrix_init (&matrix, 
        0.647919, -0.761710,
        0.761710, 0.647919,
        -208.7977, 462.0608);
    cairo_transform (cr, &matrix);

    cairo_move_to (cr, 505.80857, 746.23606);
    cairo_line_to (cr, 335.06870, 555.86488);
    cairo_line_to (cr, 91.840384, 635.31360);
    cairo_line_to (cr, 282.21157, 464.57374);
    cairo_line_to (cr, 202.76285, 221.34542);
    cairo_line_to (cr, 373.50271, 411.71660);
    cairo_line_to (cr, 616.73103, 332.26788);
    cairo_line_to (cr, 426.35984, 503.00775);
    cairo_line_to (cr, 505.80857, 746.23606);
    cairo_close_path (cr);
}

static void fill_ring (cairo_t *cr)
{
    cairo_save (cr);

    cairo_translate (cr, -90, -205);
    ring_path (cr);
    cairo_set_source_rgba (cr, 1.0, 0.0, 0.0, 0.75);
    cairo_fill (cr);

    cairo_restore (cr);
}

static void fill_star (cairo_t *cr)
{
    cairo_save (cr);

    cairo_translate (cr, -90, -205);
    star_path (cr);
    cairo_set_source_rgba (cr, 0.0, 0.0, (double) 0xae / 0xff, 0.55135137);
    cairo_fill (cr);
    
    cairo_restore (cr);
}

static void clip_to_top_and_bottom (cairo_t *cr, int width, int height)
{
    cairo_move_to (cr, 0, 0);
    cairo_line_to (cr, width, 0);
    cairo_line_to (cr, 0, height);
    cairo_line_to (cr, width, height);
    cairo_close_path (cr);
    cairo_clip (cr);
    cairo_new_path (cr);
}

static void clip_to_left_and_right (cairo_t *cr, int width, int height)
{
    cairo_move_to (cr, 0, 0);
    cairo_line_to (cr, 0, height);
    cairo_line_to (cr, width, 0);
    cairo_line_to (cr, width, height);
    cairo_close_path (cr);
    cairo_clip (cr);
    cairo_new_path (cr);
}

int main (void)
{
    cairo_t *cr, *cr_ros, *cr_sor;
    cairo_surface_t *ring_over_star, *star_over_ring, *result;

    result = cairo_image_surface_create_for_data (image, CAIRO_FORMAT_ARGB32,
                    WIDTH, HEIGHT, STRIDE);
    ring_over_star = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                    WIDTH, HEIGHT);
    star_over_ring = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                    WIDTH, HEIGHT);

    cr = cairo_create (result);

    {
        cr_ros = cairo_create (ring_over_star);
        clip_to_top_and_bottom (cr_ros, WIDTH, HEIGHT);
        fill_star (cr_ros);
        fill_ring (cr_ros);
        cairo_destroy (cr_ros);
    }

    {
        cr_sor = cairo_create (star_over_ring);
        clip_to_left_and_right (cr_sor, WIDTH, HEIGHT);
        fill_ring (cr_sor);
        fill_star (cr_sor);
        cairo_destroy (cr_sor);
    }

    cairo_set_operator (cr, CAIRO_OPERATOR_ADD);
    cairo_set_source_surface (cr, ring_over_star, 0, 0);
    cairo_paint (cr);
    cairo_set_source_surface (cr, star_over_ring, 0, 0);
    cairo_paint (cr);

    cairo_destroy (cr);

    cairo_surface_write_to_png (result, "star_and_ring.png");

    cairo_surface_destroy (result);
    cairo_surface_destroy (ring_over_star);
    cairo_surface_destroy (star_over_ring);

    return 0;
}

