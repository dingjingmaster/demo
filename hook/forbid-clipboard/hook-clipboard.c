#include <stdio.h>

#include "hook-common.h"


int XChangeProperty(void* /*Display*/dpy, int /*Window*/ w, int /*Atom*/ property, int /*Atom*/ type, int format, int mode, unsigned char* data, int nElements)
{
    return hook_x_change_property(dpy, w, property, type, format, mode, data, nElements);
}

