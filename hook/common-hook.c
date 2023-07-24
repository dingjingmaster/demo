/*************************************************************************
> FileName: hook-common.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 24 Jul 2023 08:28:48 AM CST
 ************************************************************************/
#include <X11/X.h>
#include <X11/Xlib.h>

#include <glib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <gtk/gtk.h>

static char* get_program_full_path ();
static void common_x11_forbid_drag_file_to_progress (Display* display, Window win);

bool process_is_browser (const char* progress);


static const char* gHookBrowser[] = {
    "/uosbrowser",
    "/browser",
    "/firefox",
    "/firefox-esr",
    "/qaxbrowser",
    "/qaxbrowser-safe",
    "/qaxbrowser-safe-stable",
    "/kim",
    NULL
};


bool process_is_browser (const char* progress)
{
    if (!progress) {
        return false;
    }

    for (int i = 0; gHookBrowser[i]; ++i) {
        if (strstr (progress, gHookBrowser[i])) {
            return true;
        }
    }

    return false;
}

static char* get_program_full_path ()
{
    g_autofree char* cmdPath = g_strdup_printf ("/proc/%d/exe", getpid ());
    g_autofree char* programFullPath = g_file_read_link (cmdPath, NULL);

    if (!programFullPath) {
        syslog (LOG_ERR, "get program full path error! It's impossible!!!");
        exit (-1);
    }

    return g_strdup(programFullPath);
}

static void common_x11_forbid_drag_file_to_progress (Display* display, Window win)
{
    Atom atom = XInternAtom (display, "XdndAware", true);
    if (None != atom) {
        XDeleteProperty (display, win, atom);
    }
}

void common_forbid_drag_file_to_progress (Display* display, Window window, const char* programName)
{
    if (!display) return;

    g_autofree char* pName = get_program_full_path ();

    if (g_str_has_prefix (programName, "/")) {
        if (g_str_has_prefix(pName, programName)) {
            common_x11_forbid_drag_file_to_progress (display, window);
        }
    }
    else {
        gchar** pNameSet = g_strsplit (pName, "/", -1);
        for (int i = 0; NULL != pNameSet[i]; ++i) {
            if (g_str_has_prefix(pNameSet[i], programName) && NULL == pNameSet[i + 1]) {
                common_x11_forbid_drag_file_to_progress (display, window);
            }
        }
    }
}

void common_gtk_clipboard_uri_received(GtkClipboard* clipboard, char** uris, gpointer data)
{
    if (!clipboard || !uris) {
        return;
    }

    GString* str = g_string_new(NULL);
    for (int i = 0; uris[i]; ++i) {
        g_string_append (str, uris[i]);
        g_string_append (str, "{]");
    }

    g_autofree char* sstr = g_string_free (str, false);

    syslog(LOG_ERR, "URIS: %s", sstr);
}

void common_gtk_clipboard_text_received(GtkClipboard* clipboard, char* text, gpointer data)
{
    if (!clipboard || !text) {
        return;
    }

    syslog(LOG_ERR, "text: %s", text);
}


