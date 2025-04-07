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

#define HOOK_FUNC(ret, func, ...) \
	extern ret func (__VA_ARGS__); \

static inline void* get_function_ptr(const char* funcName)
{
	return dlsym(RTLD_NEXT, funcName);
}

bool	wayland_check_is_set_title			(uint32_t opcode);
void*	wayland_default_proxy_marshal_flags	(void* proxy, uint32_t opcode, const void* interface, uint32_t version, uint32_t flags, va_list ap);


#endif
