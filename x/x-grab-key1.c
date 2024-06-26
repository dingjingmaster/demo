/*************************************************************************
> FileName: x-grab-key1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 18 Dec 2023 03:40:52 PM CST
 ************************************************************************/
#include <X11/XKBlib.h>
#include <X11/extensions/XInput2.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const char * DEFAULT_DISPLAY    = NULL;
const bool   DEFAULT_PRINT_UP   = false;

int printUsage() 
{
    printf("\
USAGE: xkbcat [-display <display>] [-up]\n\
    display  target X display                   (default %s)\n\
    up       also print key-ups                 (default %s)\n",
        DEFAULT_DISPLAY, (DEFAULT_PRINT_UP ? "yes" : "no") );
    exit(0);
}

int main(int argc, char * argv[]) 
{

    const char * xDisplayName = DEFAULT_DISPLAY;
    bool         printKeyUps  = DEFAULT_PRINT_UP;

    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-help"))     printUsage();
        else if (!strcmp(argv[i], "-up"))       printKeyUps = true;
        else if (!strcmp(argv[i], "-display")) {
            // Read next entry to find value
            ++i;
            if (i >= argc) {
                fprintf(stderr, "No value given to option `-display`\n");
                printUsage();
                exit(5);
            }
            xDisplayName = argv[i];
        }
        else { printf("Unexpected argument `%s`\n", argv[i]); printUsage(); }
    }

    // Connect to X display
    Display * disp = XOpenDisplay(xDisplayName);
    if (NULL == disp) {
        fprintf(stderr, "Cannot open X display '%s'\n", xDisplayName);
        exit(1);
    }

    int xiOpcode;
    { 
        // Test for XInput 2 extension
        int queryEvent, queryError;
        if (! XQueryExtension(disp, "XInputExtension", &xiOpcode, &queryEvent, &queryError)) {
            fprintf(stderr, "X Input extension not available\n");
            exit(2);
        }
    }

    {
        // Request XInput 2.0, to guard against changes in future versions
        int major = 2, minor = 0;
        int queryResult = XIQueryVersion(disp, &major, &minor);
        if (queryResult == BadRequest) {
            fprintf(stderr, "Need XI 2.0 support (got %d.%d)\n", major, minor);
            exit(3);
        }
        else if (queryResult != Success) {
            fprintf(stderr, "XIQueryVersion failed!\n");
            exit(4);
        }
    }
    
    {
        // Register to receive XInput events
        Window root = DefaultRootWindow(disp);
        XIEventMask m;
        m.deviceid = XIAllMasterDevices;
        m.mask_len = XIMaskLen(XI_LASTEVENT);
        m.mask = (unsigned char*)calloc(m.mask_len, sizeof(char));

        // Raw key presses correspond to physical key-presses, without
        // processing steps such as auto-repeat.
        XISetMask(m.mask, XI_RawKeyPress);
        if (printKeyUps) XISetMask(m.mask, XI_RawKeyRelease);
        XISelectEvents(disp, root, &m, 1 /*number of masks*/);
        XSync(disp, false);
        free(m.mask);
    }

    int xkbOpcode, xkbEventCode;
    {
        // Test for Xkb extension
        int queryError, majorVersion, minorVersion;
        if (! XkbQueryExtension(disp, &xkbOpcode, &xkbEventCode, &queryError, &majorVersion, &minorVersion)) {
            fprintf(stderr, "Xkb extension not available\n");
            exit(2);
        }
    }

    // Register to receive events when the keyboard's keysym group changes.
    // Keysym groups are normally used to switch keyboard layouts.  The
    // keyboard continues to send the same keycodes (numeric identifiers of
    // keys) either way, but the active keysym group determines how those map
    // to keysyms (textual names of keys).
    XkbSelectEventDetails(disp, XkbUseCoreKbd, XkbStateNotify, XkbGroupStateMask, XkbGroupStateMask);
    int group;
    { // Determine initial keysym group
        XkbStateRec state;
        XkbGetState(disp, XkbUseCoreKbd, &state);
        group = state.group;
    }

    while ("forever") {
        XEvent event;
        XGenericEventCookie *cookie = (XGenericEventCookie*)&event.xcookie;
        XNextEvent(disp, &event);

        if (XGetEventData(disp, cookie)) {
            // Handle key press and release events
            if (cookie->type == GenericEvent && cookie->extension == xiOpcode) {
                if (cookie->evtype == XI_RawKeyRelease || cookie->evtype == XI_RawKeyPress) {
                    XIRawEvent *ev = cookie->data;

                    // Ask X what it calls that key; skip if unknown.
                    // Ignore shift-level argument, to show the "basic" key
                    // regardless of what modifiers are held down.
                    KeySym s = XkbKeycodeToKeysym(disp, ev->detail, group, 0 /*shift level*/);

                    // Non-zero keysym groups are "overlays" on the base (`0`)
                    // group.  If the current group has no keysym for this
                    // keycode, defer to the base group instead.  (This usually
                    // happens with common shared keys like Return, Backspace,
                    // or numeric keypad keys.)
                    if (NoSymbol == s) {
                        if (group == 0) continue;
                        else {
                            s = XkbKeycodeToKeysym(disp, ev->detail,
                                    0 /* base group */, 0 /*shift level*/);
                            if (NoSymbol == s) continue;
                        }
                    }
                    char *str = XKeysymToString(s);
                    if (NULL == str) continue;

                    // Output line
                    if (printKeyUps) printf("%s", cookie->evtype == XI_RawKeyPress ? "+" : "-");
                    printf("%s\n", str);
                    fflush(stdout);
                }
            }
            // Release memory associated with event data
            XFreeEventData(disp, cookie);
        }
        else {
            // No extra data to release; `event` contains everything.
            // Handle keysym group change events
            if (event.type == xkbEventCode) {
                XkbEvent *xkbEvent = (XkbEvent*)&event;
                if (xkbEvent->any.xkb_type == XkbStateNotify) {
                    group = xkbEvent->state.group;
                }
            }
        }
    }
}
