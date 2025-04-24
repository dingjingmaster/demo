/*************************************************************************
> FileName: hook-screenshot.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月24日 星期四 13时42分04秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>


void gdk_pixbuf_save_to_stream_async (void* pixbuf, void* stream, const char* type, void* cancellable, void* callback, void* udata, ...)
{
	printf("no screenshot\n");
	exit(0);
}
