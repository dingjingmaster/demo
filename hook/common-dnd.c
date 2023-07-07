/*************************************************************************
> FileName: ./common-dnd.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 06 Jul 2023 03:42:07 PM CST
 ************************************************************************/
#include <X11/X.h>
#include <X11/Xlib.h>

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "c-log.h"
#include "c-log.c"

static char* get_program_full_path ();

void xdnd_drop                          (void* event1);
bool check_event                        (void* event1, const char* programName, int eventType, const char* eventName);


void xdnd_drop (void* event/*XEvent*/)
{
    XClientMessageEvent* ev = (XEvent*) event;

    if (!event) return;

    XDeleteProperty (ev->display, ev->window, XInternAtom(ev->display, "XdndActionList", false));
    XDeleteProperty (ev->display, ev->window, XInternAtom(ev->display, "XdndDirectSave0", false));
    XDeleteProperty (ev->display, ev->window, XInternAtom(ev->display, "XdndActionDirectSave", false));

    //ev->message_type = XInternAtom (ev->display, "XdndPosition", false);
    //ev->message_type = XInternAtom (ev->display, "XdndFinished", false);
}

// 
bool check_event (void* event1, const char* programName, int eventType, const char* eventName)
{
    XEvent* ev = (XEvent*) event1;

    if (!event1 || !eventName || ev->type != eventType) return false;

    g_autofree char* pname = get_program_full_path();
    if (!strstr(pname, programName)) return false;

    switch (ev->type) {
        case 2  : {
            // "KeyPress"           
            break;
        }
        case 3  : {
            // "KeyRelease"
            break;
        }
        case 4  : {
            // "ButtonPress"
            break;
        }
        case 5  : {
            // "ButtonRelease"
            break;
        }
        case 6  : {
            // "MotionNotify"
            break;
        }
        case 7  : {
            // "EnterNotify"
            break;
        }
        case 8  : {
            // "LeaveNotify"
            break;
        }
        case 9  : {
            // "FocusIn"
            break;
        }
        case 10 : {
            // "FocusOut"
            break;
        }
        case 11 : {
            // "KeymapNotify"
            break;
        }
        case 12 : {
            // "Expose"
            break;
        }
        case 13 : {
            // "GraphicsExpose"
            break;
        }
        case 14 : {
            // "NoExpose"
            break;
        }
        case 15 : {
            // "VisibilityNotify"
            break;
        }
        case 16 : {
            // "CreateNotify"
            break;
        }
        case 17 : {
            // "DestroyNotify"
            break;
        }
        case 18 : {
            // "UnmapNotify"
            break;
        }
        case 19 : {
            // "MapNotify"
            break;
        }
        case 20 : {
            // "MapRequest"
            break;
        }
        case 21 : {
            // "ReparentNotify"
            break;
        }
        case 22 : {
            // "ConfigureNotify"
            break;
        }
        case 23 : {
            // "ConfigureRequest"
            break;
        }
        case 24 : {
            // "GravityNotify"
            break;
        }
        case 25 : {
            // "ResizeRequest"
            break;
        }
        case 26 : {
            // "CirculateNotify"
            break;
        }
        case 27 : {
            // "CirculateRequest"
            break;
        }
        case 28 : {
            // "PropertyNotify"
            break;
        }
        case 29 : {
            // "SelectionClear"
            break;
        }
        case 30 : {
            // "SelectionRequest"
            break;
        }
        case 31 : {
            // "SelectionNotify"
            break;
        }
        case 32 : {
            // "ColormapNotify"
            break;
        }
        case 33 : {
            // "ClientMessage"
            XClientMessageEvent* ev = (XClientMessageEvent*) event1;
            char* messageTypeT = XGetAtomName(ev->display, ev->message_type);
            return (0 == g_strcmp0 (messageTypeT, eventName));
        }
        case 34 : {
            // "MappingNotify"
            break;  
        }
        case 35 : {
            // "GenericEvent"
            break;
        }
        case 36 : {
            // "LASTEvent"
            break;
        }
        default : {
            // "unknown"
            break;
        }
    }

    return false;
}


char* get_program_full_path ()
{
    g_autofree char* cmdPath = g_strdup_printf ("/proc/%d/exe", getpid());
    g_autofree char* programFullPath = g_file_read_link (cmdPath, NULL);

    if (programFullPath) {
        return g_strdup(programFullPath);
    }

    return NULL;
}
