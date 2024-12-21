#ifndef _HOOK_COMMON_H_
#define _HOOK_COMMON_H_
#include <stdint.h>
#include <stdbool.h>


int hook_x_change_property(void* /*Display*/dpy, int /*Window*/ w, int /*Atom*/ property, int /*Atom*/ type, int format, int mode, unsigned char* data, int nElements);

void x_clear_clipboard   (void* display);

#endif
