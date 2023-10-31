/*************************************************************************
> FileName: xevent-type.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 31 Oct 2023 05:04:48 PM CST
 ************************************************************************/
#ifndef _XEVENT_TYPE_H
#define _XEVENT_TYPE_H
#include <X11/Xlib.h>

const char* getEventType(XEvent* event);

#endif
