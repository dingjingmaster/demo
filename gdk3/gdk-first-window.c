/*************************************************************************
> FileName: gdk-first-window.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2020年01月06日 星期一 21时30分18秒
 ************************************************************************/
#include <gdk/gdk.h>
#include <glib.h>

int main (int argc, char *argv[]) {
    GdkWindow *window;

    /* a set of "attributes" for our windows. This struct holds things
     * like width, height etc.
     */
    GdkWindowAttr attributes;

    /* a bitmask of the attributes you actually want to read out of the
     * attributes thing above
     */
    gint attributes_mask;

    /* GDK apps need a main loop to wait for events and so forth. This main
     * loop has a struct which tells you about it called "GMainLoop". We need
     * one of them.
     */
    GMainLoop *mainloop;

    /* initialise gdk. We pass in argc and argv - it uses them for info like
     * display on X and so forth. Die if GDK can't initialize.
     * 
     * Always initialise gdk before working with it. Stuff breaks otherwise.
     */
    if (!gdk_init_check (&argc, &argv)) {
        return FALSE;
    }

    /* gdk_rgb_init() is a function which I can only guess sets up the
     * true colour colour map. It returns void so we can't check its
     * return value.
     */
    gdk_rgb_init();

    /* set the attributes for the window. Lets create a 400x400 top-level
     * window.
     */
    attributes.window_type = GDK_WINDOW_TOPLEVEL;
    attributes.width = 400;
    attributes.height = 400;
    attributes.wclass = GDK_INPUT_OUTPUT;

    /* Get a true-colour colour map. GDK also supports indexed colour maps
     * but they aren't so interesting for us.
     */
    attributes.colormap = gdk_rgb_get_cmap ();

    /* set our mask so that the "colormap" element is used.
     */
    attributes_mask = GDK_WA_COLORMAP;

    /* create the window.
     */
    window = gdk_window_new (NULL, &attributes, attributes_mask);

    gdk_window_set_keep_below (window, TRUE);

    /* show the window.
     */
    gdk_window_show (window);

    /* Create a new main loop object. The True is to say that the structure
     * should be initialised to say that it is running.
     */
    mainloop = g_main_new (TRUE);

    /* Run that main loop
     */
    g_main_run (mainloop);
}
