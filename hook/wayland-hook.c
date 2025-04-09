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
#include <string.h>
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

	return listener(proxy, implementation, data);
}

typedef void* (*WlShmBufferGetData) (void* buffer);
void* wl_shm_buffer_get_data (void* buffer)
{
	WlShmBufferGetData shm = get_function_ptr("wl_shm_buffer_get_data");
	if (!shm) {
		logi("wl_shm_buffer_get_data error!");
		return NULL;
	}

	void* data = shm(buffer);
	if (data) {
		logi("SHM=> %s", (char*) data);
	}

	return data;
}

#if 1
static const char* gsClipboard = "\0andsec\0";
static const int gsClipboardLen = 9;
typedef ssize_t (*Write) (int fd, const void* buf, size_t count);
ssize_t write (int fd, const void* buf, size_t count)
{
	Write w = (Write) get_function_ptr("write");
	if (!w) {
		logi("write error!");
		return 0;
	}

	ssize_t ret = 0;
	uint32_t sfd = wayland_get_send_fd ();
	if (fd == sfd) {
		if (0 != memcmp(buf, gsClipboard, gsClipboardLen)) {
			ret = w(fd, gsClipboard, gsClipboardLen);
		}
		logi("[%d] [SEND]: %s", fd, (char*) buf);
	}


	ret += w (fd, buf, count);

	return ret;
}

typedef ssize_t (*Read) (int fd, void* buf, size_t count);
ssize_t read (int fd, void* buf, size_t count)
{
	Read r = (Read) get_function_ptr("read");
	if (!r) {
		logi("read error!");
		return 0;
	}

	ssize_t ret = r (fd, buf, count - 1);
	if (hook_check_is_pipe(fd) && ret > gsClipboardLen && 0 == memcmp(buf, gsClipboard, gsClipboardLen)) {
		memmove(buf, buf + gsClipboardLen, ret - gsClipboardLen);
		memset(buf + ret - gsClipboardLen + 1, 0, 1);
		ret -= gsClipboardLen;
		logi("[%d] [RECV]: %s", fd, (char*)buf + gsClipboardLen);
	}

	return ret;
}
#endif
