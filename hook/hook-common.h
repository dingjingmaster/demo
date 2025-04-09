/*************************************************************************
> FileName: hook-common.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月07日 星期一 10时13分34秒
 ************************************************************************/
#ifndef _HOOK_COMMON_H
#define _HOOK_COMMON_H

#include <dlfcn.h>
#include <stdint.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>


#ifndef RTLD_NEXT
#define RTLD_NEXT (void*) -1
#endif

#define LOGI

#ifdef LOGI
//#define logi(x) syslog(LOG_ERR, "[HOOK] %s %d %s %s", __FILE__, __LINE__, __func__, x)
#define logi(...) syslog(LOG_ERR, __VA_ARGS__)
#else
#define logi(x)
#endif


static inline void* get_function_ptr(const char* funcName)
{
	return dlsym(RTLD_NEXT, funcName);
}

typedef struct _WaylandCore WaylandCore;

void		wayland_get_proxy							(void* dsp);
bool		wayland_check_is_set_title					(uint32_t opcode);
void		wayland_init_clipboard						(WaylandCore* dsp);
void		wayland_append_wl_data_source_add_listener	(void* dataSrc, void* dataSrcListener, void* udata);
void*		wayland_default_proxy_marshal_flags			(void* proxy, uint32_t opcode, const void* interface, uint32_t version, uint32_t flags, va_list ap);
uint32_t	wayland_get_send_fd							();
uint32_t	wayland_get_recv_fd							();

bool        hook_check_is_pipe							(uint32_t fd);
void		hook_pipe_fds								(uint32_t fd1, uint32_t fd2);
void		hook_close_fd								(int fd);

void    common_set_mem_write						(void* mem);
void    common_set_mem_readonly						(void* mem);

void    debug_fd_info								(int fd);


#endif
