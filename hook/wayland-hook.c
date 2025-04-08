/*************************************************************************
> FileName: wayland-hook.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月07日 星期一 10时04分09秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdarg.h>

#include "hook-common.h"


extern WaylandCore* gsWaylandCorePtr;

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

// struct wl_display* wl_display_connect(const char *name);
typedef void* (*WlDisplayConnect) (const char*);
void* wl_display_connect(const char *name)
{
	WlDisplayConnect dsp = get_function_ptr("wl_display_connect");
	if (!dsp) {
		logi("wl_display_connect error!");
		return NULL;
	}

	void* disp = dsp(name);
	wayland_get_proxy(disp);

	logi("wl_display_connect");

	return disp;
}

// int wl_proxy_add_listener(struct wl_proxy *proxy, void (**implementation)(void), void *data);
typedef int (*WlProxyAddListener) (void* proxy, void (**implementation)(void), void *data);
int wl_proxy_add_listener(void* proxy, void (**implementation)(void), void *data)
{
	WlProxyAddListener listener = get_function_ptr("wl_proxy_add_listener");
	if (!listener) {
		return -1;
	}

	wayland_append_wl_data_source_add_listener(proxy, implementation, data);
	//logi("wl_proxy_add_listener -- %p, %p", proxy, implementation);

	return listener(proxy, implementation, data);
}

#if 0
typedef ssize_t (*Write) (int fd, const void* buf, size_t count);
ssize_t write (int fd, const void* buf, size_t count)
{
	Write w = (Write) get_function_ptr("write");
	if (!w) {
		logi("write error!");
		return 0;
	}

	logi("[%d]: %s", fd, (char*) buf);
	debug_fd_info (fd);

	return w (fd, buf, count);
}
#endif
