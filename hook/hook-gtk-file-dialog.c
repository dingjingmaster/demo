/*************************************************************************
> FileName: hook-gtk-file-dialog.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 07 Jun 2023 01:43:18 PM CST
 ************************************************************************/
#include <stdio.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "c-log.h"

#ifndef RTLD_NEXT
#define RTLD_NEXT (void*)-1
#endif

typedef void* (*GtkFileChooserGetFileNames) (void* chooser);

#if 0
void* gtk_file_chooser_get_filenames (void* chooser)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_filenames");
    if (old == NULL) {
        return NULL;
    }
    loge("OK1 files");

    sleep (10);
    return old(chooser);
}

void* gtk_file_chooser_get_filename (void* chooser)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    GtkFileChooserGetFileNames old = (GtkFileChooserGetFileNames) dlsym (RTLD_NEXT, "gtk_file_chooser_get_filename");
    if (old == NULL) {
        return NULL;
    }
    loge("OK1 file");
    sleep (10);
    return old(chooser);
}
#endif 

// no
void gtk_target_list_add (void* list, unsigned long atom, unsigned int flags, unsigned int info)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    loge("OK1 file");
}

// no
char** gtk_selection_data_get_uris(void* s)
{
    return NULL;
}

void* gtk_drag_begin_with_coordinates (void* widget, void* targets, long actions, int button, void* event, int x, int y)
{
    return NULL;
}

int gtk_drag_check_threshold (void* widget, int sx, int sy, int tx, int ty)
{
    return 0;
}

void gtk_drag_dest_add_image_targets (void* widget)
{
}

void gtk_drag_dest_add_uri_targets(void* w)
{
}

void gtk_drag_dest_add_text_targets(void* w)
{
}

long gtk_drag_dest_find_target (void* widget, void* context, void* targetLists)
{
    return 0;
}

void* gtk_drag_dest_get_target_list (void* widget)
{
    return NULL;
}

void gtk_drag_dest_set_target_list (void* widget, void* list)
{
}

void gtk_drag_finish (void* context, bool success, bool del, unsigned int tim)
{
}

void gtk_drag_get_data (void* w, void* i, long t, unsigned int tim)
{
}

void* gtk_drag_get_source_widget (void* context)
{
    return NULL;
}

void gtk_drag_source_add_uri_targets(void* w)
{
}

void gtk_drag_source_add_text_targets(void* w)
{
}

void* gtk_drag_source_get_drag (void* s)
{
    return NULL;
}

void* gtk_drag_source_new (void)
{
    return NULL;
}

void gtk_drag_source_set (void* w, unsigned long button, const void* targets, int ntargets, unsigned long actions)
{
}

void gtk_drag_source_set_content (void* s, void* c)
{
}

void gtk_drag_source_set_target_list (void* w, void* tl)
{
}

void* gtk_drop_down_new (void* model, void* exp)
{
    return NULL;
}

void* gtk_drop_down_new_from_strings (const char* const * strings)
{
    return NULL;
}

void* gtk_drop_target_get_current_drop (void* s)
{
    return NULL;
}

const void* gtk_drop_target_get_value (void* s)
{
    return NULL;
}

void* gtk_drop_target_get_drop (void* s)
{
    return NULL;
}

typedef uint64_t (*XInternAtomPtr) (void* disp, const char* name, bool exists);

uint64_t XInternAtom (void* disp, const char* name, bool exists)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XInternAtomPtr old = (XInternAtomPtr) dlsym (RTLD_NEXT, "XInternAtom");
    if (old == NULL) {
        return -1;
    }
    loge("XInternAtom: %s", name);

    return old(disp, name, exists);
}

typedef void (*XNextEventPtr) (void* disp, void* event);

void XNextEvent (void* disp, void* event)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XNextEventPtr old = (XNextEventPtr) dlsym (RTLD_NEXT, "XNextEvent");
    if (old == NULL) {
        return ;
    }

    XGetAtomNamePtr atomName = (XGetAtomNamePtr) dlsym (RTLD_NEXT, "XGetAtomName");
    if (atomName == NULL) {
	loge ("XGetAtomName error");
        return 0;
    }

    old(disp, event);
    int* p = (int*) event;

    const char* str = NULL;

    switch (*p) {
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
        case 33 :       str = "ClientMessage";      break;
        case 34 :       str = "MappingNotify";      break;  
        case 35 :       str = "GenericEvent";       break;
        case 36 :       str = "LASTEvent";          break;
        default :       str = "unknown";            break;
    }

    // ClientMessage
    if (33 == *p) {
    	//loge ("XNextEvent: %d - %s - %s", *p, str, atomName(disp, ));
    }
    else {
    	loge ("XNextEvent: %d - %s", *p, str);
    }

    return;
}


typedef char* (*XGetAtomNamePtr)(void* display/*Display*/, int64_t atom /*Atom*/);
typedef int (*XChangePropertyPtr) (void* display/*Diaplay*/, int64_t /*Window*/, int64_t property/*Atom*/, int64_t type/*Atom*/, int format, int mode, const unsigned char* data, int nes);
typedef int (*XGetWindowPropertyPtr) (void* display/*Display*/, int64_t window/*Window*/, int64_t property/*Atom*/, long offset, long length, bool del, int64_t reqType/*Atom*/, int64_t* retType/*Atom*/, int* retFormatType, unsigned long* retNElem, unsigned long* retByte, unsigned char** retProp);

int XChangeProperty (void* display, int64_t window, int64_t property, int64_t type, int format, int mode, const unsigned char* data, int npositions)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XChangePropertyPtr old = (XChangePropertyPtr) dlsym (RTLD_NEXT, "XChangeProperty");
    if (old == NULL) {
        return 0;
    }

    XGetAtomNamePtr atomName = (XGetAtomNamePtr) dlsym (RTLD_NEXT, "XGetAtomName");
    if (atomName == NULL) {
	loge ("XGetAtomName error");
        return 0;
    }

    loge("XChangeProperty: '%s', property: '%s', type: '%s'", (data ? data : "<null>"), atomName(display, property), atomName(display, type));

    return old(display, window, property, type, format, mode, data, npositions);
}

int XGetWindowProperty (void* display/*Display*/, int64_t window/*Window*/, int64_t property/*Atom*/, long offset, long length, bool del, int64_t reqType/*Atom*/, int64_t* retType/*Atom*/, int* retFormatType, unsigned long* retNElem, unsigned long* retByte, unsigned char** retProp)
{
    log_init(LOG_TYPE_FILE, LOG_DEBUG, LOG_ROTATE_FALSE, 2<<30, "/tmp/", "hook", "log"); 
    XGetWindowPropertyPtr old = (XGetWindowPropertyPtr) dlsym (RTLD_NEXT, "XGetWindowProperty");
    if (old == NULL) {
        return 0;
    }

    XGetAtomNamePtr atomName = (XGetAtomNamePtr) dlsym (RTLD_NEXT, "XGetAtomName");
    if (atomName == NULL) {
	loge ("XGetAtomName error");
        return 0;
    }

    int ret = old (display, window, property, offset, length, del, reqType, retType, retFormatType, retNElem, retByte, retProp);

    loge ("XGetWindowProperty -- property: '%s', prop: '%s'", atomName(display, property), *retProp);

    return ret;
}


