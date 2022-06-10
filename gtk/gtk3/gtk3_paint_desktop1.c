/*************************************************************************
> FileName: paint_desktop1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2019年12月31日 星期二 22时07分20秒
 ************************************************************************/
#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
int main (int argc, char *argv[])
{
	gtk_init(&argc, &argv);
    GdkWindow *window;
    GtkWidget *gtkwin;
  	GdkWindowAttr attributes;
  	gint attributes_mask;

	  attributes.title = "test";
	  attributes.cursor=gdk_cursor_new(GDK_GUMBY);
	  attributes.override_redirect = TRUE;
	  attributes.x = 100;
	  attributes.y = 300;
	  attributes.width = 500;
	  attributes.height = 500;
	  attributes.wclass = GDK_INPUT_OUTPUT;
	  attributes.window_type = GDK_WINDOW_TEMP;
	  attributes.event_mask= GDK_STRUCTURE_MASK
			| GDK_BUTTON_MOTION_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK
			| GDK_EXPOSURE_MASK
			| GDK_ENTER_NOTIFY_MASK
			| GDK_LEAVE_NOTIFY_MASK;
	  //attributes.type_hint = GDK_WINDOW_TYPE_HINT_NORMAL;
	  //attributes.type_hint = GDK_WINDOW_TYPE_HINT_DESKTOP;
	  attributes.type_hint = GDK_WINDOW_TYPE_HINT_UTILITY;
      attributes.visual = gdk_screen_get_system_visual(gdk_screen_get_default()) ;
      attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_CURSOR | GDK_WA_VISUAL | GDK_WA_NOREDIR ;//|GDK_WA_WMCLASS
	  window = gdk_window_new(NULL, &attributes, attributes_mask);

gdk_window_show(window);

    cairo_t *cr;
    cr = gdk_cairo_create(window);
   cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "Adobe Heiti Std",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size(cr, 130);
cairo_pattern_t *pat1;
    PangoLayout *layout;
    PangoFontDescription *font_desc;
    layout = pango_cairo_create_layout(cr);
    font_desc = pango_font_description_from_string("Sans Bold 130");
    pango_layout_set_font_description(layout, font_desc);
    pango_font_description_free(font_desc);

pat1 = cairo_pattern_create_linear(20, 150, 400, 150);
cairo_set_source_rgb(cr, 1, 0.0, 0.0);
cairo_pattern_add_color_stop_rgb (pat1, 0.0, 0, 0, 0);
cairo_pattern_add_color_stop_rgb (pat1, 0.7, 1, 1, 0);
cairo_pattern_add_color_stop_rgb (pat1, 1.0, 0, 0, 0);
cairo_set_source(cr, pat1);
cairo_move_to(cr, 30, 300);
pango_layout_set_text(layout,  "这是个测试", -1);
pango_cairo_show_layout(cr, layout);
cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
   cairo_stroke(cr);
    cairo_destroy(cr);

gtk_main();
return 0;
}

