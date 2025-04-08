/*************************************************************************
> FileName: wayland-hook.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月07日 星期一 10时04分09秒
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdarg.h>

#include "hook-common.h"


void* wlDisplay = NULL;


typedef void (*WlDispDisType)(void* dsp);
void wl_display_disconnect(void* dsp)
{
	logi("wl_display_disconnect");

	WlDispDisType disp = (WlDispDisType) get_function_ptr ("wl_display_disconnect");
	if (disp) {
		logi("wl_display_disconnect ok!");
		disp(dsp);
	}
	else {
		logi("wl_display_disconnect err!");
	}
}

// struct wl_proxy* wl_proxy_marshal_flags(struct wl_proxy *proxy, uint32_t opcode, const struct wl_interface *interface, uint32_t version, uint32_t flags, ...);
typedef void* (*WlProxyMarshalFlags) (void* proxy, uint32_t opcode, const void* interface, uint32_t version, uint32_t flags, ...);
void* wl_proxy_marshal_flags(void* proxy, uint32_t opcode, const void* interface, uint32_t version, uint32_t flags, ...)
{
	WlProxyMarshalFlags func = (WlProxyMarshalFlags) get_function_ptr ("wl_proxy_marshal_flags");
	if (!func) {
		logi("err");
		return NULL;
	}

	va_list ap;
	va_start(ap, flags);
	void* ret = wayland_default_proxy_marshal_flags(proxy, opcode, interface, version, flags, ap);
	va_end(ap);

	return ret;
}

// void wl_proxy_set_user_data(struct wl_proxy *proxy, void *user_data);
typedef void (*WlProxySetUserData)(void* proxy, void* udata);
void wl_proxy_set_user_data(void* proxy, void* udata)
{
	logi("wl_proxy_set_user_data");

	if (udata) {
		logi ("[clipboard] %s", (char*) udata);
	}

	WlProxySetUserData func = (WlProxySetUserData) get_function_ptr ("wl_proxy_set_user_data");
	if (!func) {
		logi("wl_proxy_set_user_data err");
		return;
	}


	//func (proxy, udata);
}
