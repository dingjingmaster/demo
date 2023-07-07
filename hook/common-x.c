/*************************************************************************
> FileName: common-x.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 06 Jul 2023 09:31:54 AM CST
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
#include "hook-x.h"

void xdnd_drop                          (void* event1);
void print_event_XClientMessageEvent    (void* event1);
bool check_event                        (const char* programName, const char* eventName);


void print_event_XClientMessageEvent(void* event1)
{
    XEvent* event = (XEvent*) event1;

    if (!event) return;

    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 

    const char* str = NULL;
    g_autofree char* content = NULL;


    switch (event->type) {
        case 2  :       str = "KeyPress";           break;
        case 3  :       str = "KeyRelease";         break;
        case 4  :       str = "ButtonPress";        break;
        case 5  :       str = "ButtonRelease";      break;
        case 6  :       str = "MotionNotify";       break;
        case 7  :       str = "EnterNotify";        break;
        case 8  :       str = "LeaveNotify";        break;
        case 9  :       str = "FocusIn";            break;
        case 10 :       str = "FocusOut";           break;
        case 11 :       str = "KeymapNotify";       break;
        case 12 :       str = "Expose";             break;
        case 13 :       str = "GraphicsExpose";     break;
        case 14 :       str = "NoExpose";           break;
        case 15 :       str = "VisibilityNotify";   break;
        case 16 :       str = "CreateNotify";       break;
        case 17 :       str = "DestroyNotify";      break;
        case 18 :       str = "UnmapNotify";        break;
        case 19 :       str = "MapNotify";          break;
        case 20 :       str = "MapRequest";         break;
        case 21 :       str = "ReparentNotify";     break;
        case 22 :       str = "ConfigureNotify";    break;
        case 23 :       str = "ConfigureRequest";   break;
        case 24 :       str = "GravityNotify";      break;
        case 25 :       str = "ResizeRequest";      break;
        case 26 :       str = "CirculateNotify";    break;
        case 27 :       str = "CirculateRequest";   break;
        case 28 :       str = "PropertyNotify";     break;
        case 29 :       str = "SelectionClear";     break;
        case 30 :       str = "SelectionRequest";   break;
        case 31 :       str = "SelectionNotify";    break;
        case 32 :       str = "ColormapNotify";     break;
        case 33 : {
            str = "ClientMessage";
            XClientMessageEvent* ev = (XClientMessageEvent*) event;
            g_autofree char* data = NULL;
            if (32 == ev->format) {
                for (int i = 0; i < 5; ++i) {
                    char* ddd = data;
                    char* lll = (char*) (ev->data.l[i]);
                    logd ("%s", lll);
                    if (NULL == ddd) {
                        data = g_strdup_printf ("%s", lll ? lll : "<null>");
                    }
                    else {
                        data = g_strdup_printf ("%s{]%s", ddd, lll ? lll : "<null>");
                    }
                    g_free (ddd);
                }
            }

            content = g_strdup_printf("type: %d\n\
                                    serial: %ld\n\
                                    send_event: %s\n\
                                    Display: %p\n\
                                    Window: %ld\n\
                                    message_type: %s\n\
                                    format: %d\n\
                                    data: %s",
                                    ev->type,
                                    ev->serial,
                                    (ev->send_event ? "true" : "false"),
                                    ev->display,
                                    ev->window,
                                    XGetAtomName(ev->display, ev->message_type),
                                    ev->format,
                                    (data ? data : "<null>"));
            break;
        }
        case 34 :       str = "MappingNotify";      break;  
        case 35 :       str = "GenericEvent";       break;
        case 36 :       str = "LASTEvent";          break;
        default :       str = "unknown";            break;
    }

    logi ("XNextEvent: %d - %s\n%s", event->type, str, (content ? content : "<null>"));

    return;

}
