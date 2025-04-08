/*************************************************************************
> FileName: hook-common.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月07日 星期一 11时00分38秒
 ************************************************************************/
#include "hook-common.h"

#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <wayland-util.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>

#include "hook-macros.h"
#include "wayland-private.h"

struct wl_event_queue {
	struct wl_list event_list;
	struct wl_list proxy_list; /**< struct wl_proxy::queue_link */
	struct wl_display *display;
	char *name;
};

struct wl_proxy
{
	struct wl_object object;
	struct wl_display *display;
	struct wl_event_queue *queue;
	uint32_t flags;
	int refcount;
	void *user_data;
	wl_dispatcher_func_t dispatcher;
	uint32_t version;
	const char * const *tag;
	struct wl_list queue_link; /**< in struct wl_event_queue::proxy_list */
};

typedef void (*WaylandClipboardSend)(void* data, struct wl_data_source* wl_data_source, const char* mimeType, int32_t fd);

struct _WaylandCore 
{
	struct wl_display*				dsp;
	struct wl_registry*				resgistry;

	void*							clipboardProxy;

	// struct wl_data_source*,  const struct wl_data_source_listener*,  void* udata 
	void*							curWlDataSource;
	void*							curWlDataSourceListener;
	void*							curWlDataSourceListenerData;
	void*							wlDataSource[10];
	void*							wlDataSourceListener[10];
	void*							wlDataSourceListenerData[10];

	// static void send_handler(void *data, struct type *proxy, const char *mime_type, int fd);
	WaylandClipboardSend			wlDataSourceSendFunc;
	// end
} gsWaylandCore = {
0
};

WaylandCore* gsWaylandCorePtr = &gsWaylandCore;

static char* get_program_full_path ();
static void* c_realloc (void* ptr, uint64_t size);
static char* c_file_read_link(const char *filename);
static char* c_strdup_printf (const char* format, ...);
static char* c_strdup_vprintf (const char* format, va_list args);
static bool c_str_has_prefix (const char* str, const char* prefix);
static bool c_str_has_suffix (const char* str, const char* suffix);
static int c_snprintf (char* str, long n, char const *format, ...);
static int c_vasprintf (char** str, char const* format, va_list args);

static void hc_update_gedit_title (const char* str);
static void hc_update_title(uint32_t opcode, const char* signature, const char* str);
static void hc_get_clipboard(void* proxy, uint32_t opcode, const char* signature, const char* str);
static void hc_wayland_clipboard_send(void* data, struct wl_data_source* wl_data_source, const char* mimeType, int32_t fd);

static const char* my_get_next_argument(const char *signature, struct argument_details *details);
static void my_wl_argument_from_va_list(uint32_t opcode, void* proxy, const char *signature, union wl_argument *args, int count, va_list ap);


char gsCurrentTitle[PATH_MAX + PATH_MAX] = {0};


bool wayland_check_is_set_title(uint32_t opcode)
{
	return opcode == WL_SHELL_SURFACE_SET_TITLE;
}

void* wayland_default_proxy_marshal_flags (void* proxy, uint32_t opcode, const void* interface, uint32_t version, uint32_t flags, va_list ap)
{
	union wl_argument args[20];

	struct wl_proxy* proxyT = proxy;

	my_wl_argument_from_va_list(opcode, proxy, proxyT->object.interface->methods[opcode].signature, args, 20, ap);

	//logi("proxy: %p", proxy);

	return wl_proxy_marshal_array_flags(proxy, opcode, interface, version, flags, args);
}

void wayland_get_proxy (void* dsp)
{
	if (!dsp) { return; }

	gsWaylandCore.dsp = dsp;
	gsWaylandCore.resgistry = wl_display_get_registry(dsp);
}

void wayland_init_clipboard (WaylandCore* dsp)
{
	if (!dsp) { return; }
}

void common_set_mem_write (void* mem)
{
	if (!mem) { return; }

	size_t psize = sysconf(_SC_PAGESIZE);
	uintptr_t addr = (uintptr_t) mem;
	uintptr_t pstart = addr & ~(psize - 1);
	if (0 != mprotect((void*) pstart, psize, PROT_READ | PROT_WRITE)) {
		perror("mprotect");
	}
}

void common_set_mem_readonly (void* mem)
{
	size_t psize = sysconf(_SC_PAGESIZE);
	uintptr_t addr = (uintptr_t) mem;
	uintptr_t pstart = addr & ~(psize - 1);
	if (0 != mprotect((void*) pstart, psize, PROT_READ)) {
		perror("mprotect");
	}
}

void debug_fd_info (int fd)
{
	char* pp = c_strdup_printf ("/proc/%d/fd/%d", getpid (), fd); 
	if (pp) {
		char* ff = c_file_read_link (pp); 
		if (ff) {
			if (c_str_has_prefix(ff, "pipe"))
			logi("==>ff: %s", ff);
			c_free(ff);
		}
		c_free(pp);
	}
}






static const char* my_get_next_argument(const char *signature, struct argument_details *details)
{
	details->nullable = 0;

	for(; *signature; ++signature) {
		switch(*signature) {
		case WL_ARG_INT:
		case WL_ARG_UINT:
		case WL_ARG_FIXED:
		case WL_ARG_STRING:
		case WL_ARG_OBJECT:
		case WL_ARG_NEW_ID:
		case WL_ARG_ARRAY:
		case WL_ARG_FD:
			details->type = *signature;
			return signature + 1;
		case '?':
			details->nullable = 1;
		}
	}
	details->type = '\0';

	return signature;
}

static void my_wl_argument_from_va_list(uint32_t opcode, void* proxy, const char *signature, union wl_argument *args, int count, va_list ap)
{
	int i;
	const char *sig_iter;
	struct argument_details arg;

	sig_iter = signature;
	for (i = 0; i < count; i++) {
		sig_iter = my_get_next_argument(sig_iter, &arg);

		switch(arg.type) {
		case WL_ARG_INT:
			args[i].i = va_arg(ap, int32_t);
			break;
		case WL_ARG_UINT:
			args[i].u = va_arg(ap, uint32_t);
			break;
		case WL_ARG_FIXED:
			args[i].f = va_arg(ap, wl_fixed_t);
			break;
		case WL_ARG_STRING:
			args[i].s = va_arg(ap, const char *);
			hc_update_title(opcode, signature, args[i].s);
			hc_get_clipboard(proxy, opcode, signature, args[i].s);
			break;
		case WL_ARG_OBJECT:
			args[i].o = va_arg(ap, struct wl_object *);
			break;
		case WL_ARG_NEW_ID:
			args[i].o = va_arg(ap, struct wl_object *);
			break;
		case WL_ARG_ARRAY:
			args[i].a = va_arg(ap, struct wl_array *);
			break;
		case WL_ARG_FD:
			args[i].h = va_arg(ap, int32_t);
			break;
		}
	}
}

void wayland_append_wl_data_source_add_listener(void* dataSrc, void* dataSrcListener, void* udata)
{
	if (gsWaylandCore.wlDataSource[0]) {
		for (int i = sizeof(gsWaylandCore.wlDataSource) / sizeof(gsWaylandCore.wlDataSource[0]) - 1; i >= 1; --i) {
			gsWaylandCore.wlDataSource[i] = gsWaylandCore.wlDataSource[i - 1];
			gsWaylandCore.wlDataSourceListener[i] = gsWaylandCore.wlDataSourceListener[i - 1];
			gsWaylandCore.wlDataSourceListenerData[i] = gsWaylandCore.wlDataSourceListenerData[i - 1];
		}
	}

	gsWaylandCore.wlDataSource[0] = dataSrc;
	gsWaylandCore.wlDataSourceListenerData[0] = udata;
	gsWaylandCore.wlDataSourceListener[0] = dataSrcListener;
}

static void hc_update_title(uint32_t opcode, const char* signature, const char* str)
{
	if (!signature || !str) { return; }

	static const char* gsProcs[] = {
		"gedit",
		NULL
	};

	bool isInControl = false;
	char* procPath = NULL;
	const char* procName = NULL;

	do {
		procPath = get_program_full_path();
		if (!procPath) {
			break;
		}

		for (int i = 0; gsProcs[i]; ++i) {
			if (c_str_has_suffix(procPath, gsProcs[i])) {
				isInControl = true;
				procName = gsProcs[i];
				break;
			}
		}

		if (!isInControl) break;

		if (2 == opcode && 's' == signature[0]) {
			if (0 == strcmp("gedit", procName)) {
				hc_update_gedit_title(str);
			}
		}
	} while (0);

	c_free(procPath);

	logi("opcode: %d, signature: %s, data: %s", opcode, signature, str ? str : "null");
}

static char* get_program_full_path ()
{
	char* cmdPath = c_strdup_printf ("/proc/%d/exe", getpid ()); 
	if (cmdPath) {
		char* programFullPath = c_file_read_link (cmdPath); 
		c_free(cmdPath);
		return programFullPath;
	}

     return NULL;
}

bool c_str_has_suffix (const char* str, const char* suffix)
{
    uint64_t strLen;
    uint64_t suffixLen;

    c_return_val_if_fail (str != NULL, false);
    c_return_val_if_fail (suffix != NULL, false);

    strLen = strlen (str);
    suffixLen = strlen (suffix);

    if (strLen < suffixLen) {
        return false;
    }

    return strcmp (str + strLen - suffixLen, suffix) == 0;
}

static bool c_str_has_prefix (const char* str, const char* prefix)
{
    c_return_val_if_fail (str != NULL, false);
    c_return_val_if_fail (prefix != NULL, false);

    return strncmp (str, prefix, strlen (prefix)) == 0;
}

static int c_snprintf (char* str, long n, char const *format, ...)
{
    va_list args;
    int retval;

    va_start (args, format);
    retval = vsnprintf (str, n, format, args);
    va_end (args);

    return retval;
}

static char* c_strdup_printf (const char* format, ...)
{
    char* buffer = NULL;
    va_list args;

    va_start (args, format);
    buffer = c_strdup_vprintf (format, args);
    va_end (args);

    return buffer;
}

static char* c_strdup_vprintf (const char* format, va_list args)
{
    char* str = NULL;

    c_vasprintf (&str, format, args);

    return str;
}

static int c_vasprintf (char** str, char const* format, va_list args)
{
    int len;
    c_return_val_if_fail (str != NULL, -1);

    va_list args2;

    va_copy (args2, args);

	char c[1];
    uint64_t bufLen = vsnprintf(c, 1, format, args) + 1;
    c_malloc_type(*str, char, bufLen);
    len = vsprintf (*str, format, args2);
    va_end (args2);

    if (len < 0) {
        c_free (*str);
        *str = NULL;
    }

    return len;
}

static char* c_file_read_link(const char *filename)
{
    char *buffer;
    size_t size;
    uint64_t readSize;

    c_return_val_if_fail (filename != NULL, NULL);

#define SIZE_STEP 256

    size = SIZE_STEP;
    c_malloc (buffer, size);

    while (true) {
        readSize = readlink (filename, buffer, size);
        if (readSize < 0) {
			c_free(buffer);
            return NULL;
        }

        if ((size_t) readSize < size) {
            buffer[readSize] = 0;
            return buffer;
        }

        size += SIZE_STEP;
        buffer = c_realloc (buffer, size);
    }

	return NULL;
}

static void* c_realloc (void* ptr, uint64_t size)
{
    c_return_val_if_fail(size > 0, NULL);

    if (C_LIKELY(size)) {
        void* ret = realloc(ptr, size);
        if (C_LIKELY(ret)) {
            return ret;
        }
        else {
            logi("realloc failed");
        }
    }

    c_free(ptr);

    return NULL;
}

static void hc_update_gedit_title (const char* str)
{
	c_return_if_fail(str);

	uint32_t strLen = strlen(str) - 5;

	int32_t dirB = 0;
	int32_t dirE = 0;
	int32_t fileB = 0;
	int32_t fileE = 0;

	for (int i = strLen; i >= 0; i--) {
		if (0 == strcmp(str + i, ") - gedit")) {
			dirE = i;
			continue;
		}

		if (dirE > 0 && i > 1 && str[i] == '(' && str[i - 1] == ' ' && (str[i + 1] == '/' || str[i + 1] == '~')) {
			dirB = i + 1;
			continue;
		}

		if (dirE > 0 && dirB > 0 && fileE == 0 && str[i] != ' ' && str[i + 1] == ' ' && str[i + 2] == '(') {
			fileE = i + 1;
			continue;
		}

		if (dirE > 0 && dirB > 0 && fileE > 0) {
			fileB = 0;
			break;
		}
	}

	do {
		if (fileE > 0 && fileE > fileB && dirB > 0 && dirE > dirB) {
			int32_t dirLen = 0;
			memset(gsCurrentTitle, 0, sizeof(gsCurrentTitle));
			if (dirE - dirB <= 1 && str[dirB] == '~') {
				struct passwd* pwd = getpwuid(getuid());
				if (!pwd) {
					break;
				}
				dirLen = strlen(pwd->pw_dir);
				memcpy(gsCurrentTitle, pwd->pw_dir, dirLen);
			}
			else {
				dirLen = dirE - dirB;
				memcpy(gsCurrentTitle, &str[dirB], dirLen);
			}

			memset(gsCurrentTitle + dirLen, '/', 1);

			memcpy(gsCurrentTitle + dirLen + 1, str + fileB, fileE - fileB);
			memset(gsCurrentTitle + dirLen + 1 + fileE - fileB, '\0', 1);

			if (0 != access(gsCurrentTitle, F_OK) && str[fileB] == '*') {
				fileB = 1;
				memcpy(gsCurrentTitle + dirLen + 1, str + fileB, fileE - fileB);
				memset(gsCurrentTitle + dirLen + 1 + fileE - fileB, '\0', 1);
			}
		}
	} while (0);

//	logi("dir: %d %d, file: %d %d", dirB, dirE, fileB, fileE);

	logi("title: %s", gsCurrentTitle);
}


static void hc_get_clipboard(void* proxy, uint32_t opcode, const char* signature, const char* str)
{
	c_return_if_fail(signature && str);

	if (opcode == 0 && signature[0] == 's') {
		logi("opcode: %d, signature: %s, str: %s", opcode, signature, str);
		if (0 == strcmp(str, "text/plain;charset=utf-8")) {
			if (gsWaylandCore.curWlDataSource != proxy) {
				for (int i = 0; i < sizeof(gsWaylandCore.wlDataSource) / sizeof(gsWaylandCore.wlDataSource[0]); ++i) {
					if (gsWaylandCore.wlDataSource[i] == proxy) {
						gsWaylandCore.curWlDataSource = gsWaylandCore.wlDataSource[i];
						gsWaylandCore.curWlDataSourceListener = gsWaylandCore.wlDataSourceListener[i];
						gsWaylandCore.curWlDataSourceListenerData = gsWaylandCore.wlDataSourceListenerData[i];

						// 此处替换为自己的回调函数
						//logi("proxy: %p, listener: %p, udata: %p", gsWaylandCore.curWlDataSource, gsWaylandCore.curWlDataSourceListener, gsWaylandCore.curWlDataSourceListenerData);
						struct wl_data_source_listener* wdsl = (struct wl_data_source_listener*) gsWaylandCore.curWlDataSourceListener;
						if (gsWaylandCore.wlDataSourceSendFunc != wdsl->send && hc_wayland_clipboard_send != wdsl->send) {
							gsWaylandCore.wlDataSourceSendFunc = (void*) wdsl->send;
						}

						common_set_mem_write (wdsl);
						wdsl->send = hc_wayland_clipboard_send;
						common_set_mem_readonly (wdsl);
						break;
					}
				}
			}
		}
	}
}

static void hc_wayland_clipboard_send(void* data, struct wl_data_source* wl_data_source, const char* mimeType, int32_t fd)
{
//	static pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;

//	c_return_if_fail(0 == pthread_mutex_trylock(&locker));

	logi("===========>send, fd: %d, mimeType: %s", fd, mimeType);

	if (gsWaylandCore.wlDataSourceSendFunc) {
		gsWaylandCore.wlDataSourceSendFunc(data, wl_data_source, mimeType, fd);
	}
	else {
		logi("err!");
	}

//	pthread_mutex_unlock(&locker);
}
