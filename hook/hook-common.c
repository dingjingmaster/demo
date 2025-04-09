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

typedef void (*WaylandClipboardSendCanceled)(void* data, struct wl_data_source*);
typedef void (*WaylandClipboardSend)(void* data, struct wl_data_source* wl_data_source, const char* mimeType, int32_t fd);
typedef void (*WaylandClipboardRecv)(void* data, struct wl_data_offer* wl_data_offer, const char* mime_type);

struct _WaylandCore 
{
	struct wl_display*				dsp;
	struct wl_registry*				resgistry;

	void*							clipboardProxy;

	// struct wl_data_source*,  const struct wl_data_source_listener*,  void* udata 
	void*							curWlDataSource;
	void*							curWlDataSourceListener;
	void*							curWlDataSourceListenerData;

	void*							curWlDataTarget;
	void*							curWlDataTargetListener;
	void*							curWlDataTargetListenerData;

	void*							wlDataSource[10];
	void*							wlDataSourceListener[10];
	void*							wlDataSourceListenerData[10];

	// static void send_handler(void *data, struct type *proxy, const char *mime_type, int fd);
	WaylandClipboardSend			wlDataSourceSendFunc;
	WaylandClipboardSendCanceled	wlDataSourceSendFuncCanceled;
	WaylandClipboardRecv			wlDataSourceRecvFunc;
	uint32_t						wlDataSourceSendFd;
	uint32_t						wlDataSourceRecvFd;
	// end
	
	bool							readFinished;
	uint32_t						pipePaire[1024];
	uint32_t						fdIndex[1024];
} gsWaylandCore = {
	0,
	.pipePaire = {-1, -1},
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
static void hc_get_clipboard_recv(void* proxy, uint32_t opcode, const char* signature, int32_t paramNum, uint32_t fd);
static void hc_wayland_clipboard_recv(void* data, struct wl_data_offer* wl_data_offer, const char* mimeType);

static void hc_wayland_clipboard_send_canceled(void* data, struct wl_data_source* wl_data_source);
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

	//

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

uint32_t wayland_get_send_fd ()
{
	return (hook_check_is_pipe(gsWaylandCore.wlDataSourceSendFd) ? gsWaylandCore.wlDataSourceSendFd : -1);
}

uint32_t wayland_get_recv_fd ()
{
	if (gsWaylandCore.wlDataSourceRecvFd < 0) {
		return -1;
	}

	return (hook_check_is_pipe(gsWaylandCore.wlDataSourceRecvFd) ? gsWaylandCore.wlDataSourceRecvFd : -1);
}

bool hook_check_is_pipe (uint32_t fd)
{
	bool isOK = false;
	char fileBuf[256] = {0};
	char fileTarget[32] = {0};
	snprintf(fileBuf, sizeof(fileBuf) - 1, "/proc/%d/fd/%d", getpid(), fd);
	readlink(fileBuf, fileTarget, sizeof(fileTarget) - 1);
	if (0 == memcmp(fileTarget, "pipe", 4)) {
		isOK = true;
		//logi("fd: %d is pipe", fd);
	}

	return isOK;
}

void debug_fd_info (int fd)
{
	char* pp = c_strdup_printf ("/proc/%d/fd/%d", getpid (), fd); 
	if (pp) {
		char* ff = c_file_read_link (pp); 
		if (ff) {
			//if (c_str_has_prefix(ff, "pipe"))
			//logi("==>ff: %s", ff);
			c_free(ff);
		}
		c_free(pp);
	}
}

void hook_close_fd (int fd)
{
	int num = sizeof(gsWaylandCore.pipePaire) / sizeof(gsWaylandCore.pipePaire[0]);
	if (fd >= num) {
		return;
	}

	gsWaylandCore.fdIndex[fd]--;
	if (gsWaylandCore.fdIndex[fd] <= 0) {
		gsWaylandCore.fdIndex[fd] = 0;
		gsWaylandCore.pipePaire[fd] = -1;
	}
}

void hook_pipe_fds (uint32_t fd1, uint32_t fd2)
{
	int num = sizeof(gsWaylandCore.pipePaire) / sizeof(gsWaylandCore.pipePaire[0]);
	if (fd1 >= num || fd2 >= num) {
		return;
	}

	gsWaylandCore.pipePaire[fd1] = fd2;
	gsWaylandCore.pipePaire[fd2] = fd1;

	gsWaylandCore.fdIndex[fd1] = 2;
	gsWaylandCore.fdIndex[fd2] = 2;

	//logi("%d == %d", fd1, fd2);
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
	uint32_t recvFd = -1;
	int paramNum = 0;
	const char *sig_iter;
	struct argument_details arg;

	sig_iter = signature;
	for (i = 0; i < count; i++) {
		sig_iter = my_get_next_argument(sig_iter, &arg);

		switch(arg.type) {
		case WL_ARG_INT:
			++paramNum;
			args[i].i = va_arg(ap, int32_t);
			break;
		case WL_ARG_UINT:
			++paramNum;
			args[i].u = va_arg(ap, uint32_t);
			break;
		case WL_ARG_FIXED:
			++paramNum;
			args[i].f = va_arg(ap, wl_fixed_t);
			break;
		case WL_ARG_STRING:
			++paramNum;
			args[i].s = va_arg(ap, const char *);
			hc_update_title(opcode, signature, args[i].s);
			hc_get_clipboard(proxy, opcode, signature, args[i].s);
			break;
		case WL_ARG_OBJECT:
			++paramNum;
			args[i].o = va_arg(ap, struct wl_object *);
			break;
		case WL_ARG_NEW_ID:
			++paramNum;
			args[i].o = va_arg(ap, struct wl_object *);
			break;
		case WL_ARG_ARRAY:
			++paramNum;
			args[i].a = va_arg(ap, struct wl_array *);
			break;
		case WL_ARG_FD:
			++paramNum;
			args[i].h = va_arg(ap, int32_t);
			recvFd = args[i].h;
			break;
		}
	}

	//hc_get_clipboard_recv(proxy, opcode, signature, paramNum, recvFd);

	if (paramNum == 2 && WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE == opcode) {

		//logi("WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE, proxy: %p, signature: %s", proxy, signature);
	}

	if (opcode == WL_DATA_OFFER_RECEIVE && recvFd >= 0 && recvFd != -1 && strcmp(signature, "sh") && paramNum == 2) {
		gsWaylandCore.readFinished = false;
		gsWaylandCore.wlDataSourceRecvFd = gsWaylandCore.pipePaire[recvFd];
		//logi("WL_DATA_OFFER_RECEIVE, proxy: %p, signature: %s, fd: %d", proxy, signature, recvFd);
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

	//logi("opcode: %d, signature: %s, data: %s", opcode, signature, str ? str : "null");
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

static void hc_get_clipboard_recv(void* proxy, uint32_t opcode, const char* signature, int32_t paramNum, uint32_t fd)
{
	if (opcode != WL_DATA_OFFER_RECEIVE || fd < 0 || strcmp(signature, "sh") || paramNum != 2) {
		return;
	}

	if (gsWaylandCore.curWlDataTarget != proxy) {
		for (int i = 0; i < sizeof(gsWaylandCore.wlDataSource) / sizeof(gsWaylandCore.wlDataSource[0]); ++i) {
			if (gsWaylandCore.wlDataSource[i] == proxy) {
				gsWaylandCore.curWlDataTarget = gsWaylandCore.wlDataSource[i];
				gsWaylandCore.curWlDataTargetListener = gsWaylandCore.wlDataSourceListener[i];
				gsWaylandCore.curWlDataTargetListenerData = gsWaylandCore.wlDataSourceListenerData[i];

				gsWaylandCore.wlDataSourceRecvFd = fd;

				struct wl_data_offer_listener* wdsl = (struct wl_data_offer_listener*) gsWaylandCore.curWlDataTargetListener;
				if (gsWaylandCore.wlDataSourceRecvFunc != wdsl->offer && hc_wayland_clipboard_recv != wdsl->offer) {
					gsWaylandCore.wlDataSourceRecvFunc = wdsl->offer;
				}

				common_set_mem_write (wdsl);
				wdsl->offer = hc_wayland_clipboard_recv;
				common_set_mem_readonly (wdsl);
				break;
			}
		}
	}

		//logi("===>proxy: %p, signature: %s, i: %d, recvFd: %d", proxy, signature, paramNum, fd);
		gsWaylandCore.wlDataSourceRecvFd = fd;
}

static void hc_get_clipboard(void* proxy, uint32_t opcode, const char* signature, const char* str)
{
	c_return_if_fail(signature && str);

	if (opcode == 0 && signature[0] == 's') {
		//logi("opcode: %d, signature: %s, str: %s", opcode, signature, str);
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

						if (gsWaylandCore.wlDataSourceSendFuncCanceled != wdsl->cancelled && hc_wayland_clipboard_send_canceled != wdsl->cancelled) {
							gsWaylandCore.wlDataSourceSendFuncCanceled = (void*) wdsl->cancelled;
						}

						common_set_mem_write (wdsl);
						wdsl->send = hc_wayland_clipboard_send;
						wdsl->cancelled = hc_wayland_clipboard_send_canceled;
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
	gsWaylandCore.wlDataSourceSendFd = fd;
	//logi("===========>send, fd: %d, mimeType: %s", fd, mimeType);

	if (gsWaylandCore.wlDataSourceSendFunc) {
		gsWaylandCore.wlDataSourceSendFunc(data, wl_data_source, mimeType, fd);
	}
	else {
		logi("clipboard send err!");
	}
}

static void hc_wayland_clipboard_send_canceled(void* data, struct wl_data_source* wl_data_source)
{
	gsWaylandCore.wlDataSourceSendFd = -1;
	//logi("===========>send cancelled");

	if (gsWaylandCore.wlDataSourceSendFuncCanceled) {
		gsWaylandCore.wlDataSourceSendFuncCanceled(data, wl_data_source);
	}
	else {
		logi("cancelled err!");
	}
}

static void hc_wayland_clipboard_recv(void* data, struct wl_data_offer* wl_data_offer, const char* mimeType)
{
	//logi("===========>recv, mimeType: %s", mimeType);

	if (gsWaylandCore.wlDataSourceRecvFunc) {
		gsWaylandCore.wlDataSourceRecvFunc(data, wl_data_offer, mimeType);
	}
}
