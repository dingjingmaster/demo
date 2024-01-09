/*************************************************************************
> FileName: font-list.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 09 Jan 2024 10:38:58 AM CST
 ************************************************************************/
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

int main (int argc, char* argv[])
{
    int fCount = 0;
    PangoFontMap* map = NULL;
    PangoFontFamily** f = NULL;
    PangoContext* context = NULL;

    //printf("1\n");
    context = pango_cairo_font_map_create_context(PANGO_CAIRO_FONT_MAP(pango_cairo_font_map_get_default()));
    if (!context) goto out;
    
    //printf("2\n");
    map = pango_context_get_font_map(context);
    if (!map) goto out;

    //printf("3\n");
    pango_font_map_list_families(map, &f, &fCount);

    //printf("4\n");
    for (int i = 0; (i < fCount); ++i) {
        const char* name = pango_font_family_get_name(f[i]);
        printf ("%s\n", name ? name : "");
    }

out:

    for (int i = 0; i < fCount; ++i) {
        g_object_unref(f[i]);
    }
    if (f) g_free (f);

    if (context) {g_object_unref(context);}

    return 0;
}
