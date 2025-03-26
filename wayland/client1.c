/*************************************************************************
> FileName: client1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 21 Jul 2023 03:29:18 PM CST
 ************************************************************************/
#define _POSIX_C_SOURCE 200112L
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

/* Shared memory support code */
static void randname(char *buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A' + (r & 15) + (r & 16) * 2;
        r >>= 5;
    }
}

static int create_shm_file(void)
{
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

static int allocate_shm_file(size_t size)
{
    int fd = create_shm_file();
    if (fd < 0) {
        return -1;
    }
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

/* Wayland code */
struct client_state 
{
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *wl_shm;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;

    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
};

static void wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = 
{
    .release = wl_buffer_release,
};

static struct wl_buffer* draw_frame(struct client_state *state)
{
    const int width = 640, height = 480;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw checkerboxed background */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8) {
                data[y * width + x] = 0xFF666666;
            }
            else {
                data[y * width + x] = 0xFFEEEEEE;
            }
        }
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial)
{
    struct client_state *state = data;
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_buffer *buffer = draw_frame(state);
    wl_surface_attach(state->wl_surface, buffer, 0, 0);
    wl_surface_commit(state->wl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = 
{
    .configure = xdg_surface_configure,
};

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = 
{
    .ping = xdg_wm_base_ping,
};

static void registry_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version)
{
    struct client_state *state = data;
    if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
    }
    else if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base, &xdg_wm_base_listener, state);
    }
}

static void registry_global_remove(void *data, struct wl_registry *wl_registry, uint32_t name)
{
    /* This space deliberately left blank */
}

static const struct wl_registry_listener wl_registry_listener = 
{
    .global = registry_global,
    .global_remove = registry_global_remove,
};

int main(int argc, char *argv[])
{
    struct client_state state = { 0 };
	// 1. 连接wayland服务器
    state.wl_display = wl_display_connect(NULL);
	// 2. 创建注册表对象，允许客户端列出绑定到服务器中可用的全局对象
    state.wl_registry = wl_display_get_registry(state.wl_display);
	// 3. Wayland客户端中用于注册全局对象（Global Object）监听器的关键函数，
	//    其作用是让客户端能够接收服务器端新增全局对象的通知并执行相应处理 
	//    wl_registry_add_listener(registry, &listener, user_data);
    wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
	// 4. Wayland 客户端与服务器通信中同步操作的核心函数，用于确保客户端所有挂起的请求被处理，并读取服务器返回的所有事件
	//    阻塞当前线程
    wl_display_roundtrip(state.wl_display);

	// 5. 客户端中用于创建窗口内容表面（wl_surface）的核心函数，其作用及流程如下：
	//    1. 创建窗口内容容器：通过wl_compositor接口向wayland服务器请求创建一个 wl_surface 对象，该对象代表窗口的画布。所有后续绘制操作都需要通过此表面完成
	//    2. 初始化双缓存机制：wl_surface创建时候会自动分配前后两个缓存区。客户端通过wl_surface_attach绑定图像缓存区，并通过wl_surface_commit提交绘制内容到屏幕
    state.wl_surface = wl_compositor_create_surface(state.wl_compositor);

	// 6. Wayland 协议中用于将基础表面（wl_surface）转换为可管理的窗口表面（xdg_surface）的关键函数
	//    将普通的 wl_surface 转换为 xdg_surface，使其具备窗口管理能力（如标题栏、最小化/最大化等属性）
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
	// 7. 为 xdg_surface 对象注册事件监听器的核心函数
	//    1. 事件监听注册：该函数通过将自定义的监听器结构体绑定到 xdg_surface，实现对窗口状态变化（如大小调整、配置更新）的响应。例如，当窗口被移动或调整大小时，服务器会发送 configure 事件，触发监听器回调
	//    2. 生命周期管理：通过监听器处理窗口的初始化、销毁等关键事件，确保客户端与服务器状态同步。例如，configure 事件需通过 ack_configure 确认处理完成，避免界面卡顿
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
	// 8. xdg_surface_get_toplevel 是 Wayland 协议中用于将 xdg_surface 转换为顶级窗口对象（xdg_toplevel）的核心函数，属于 xdg-shell 扩展协议的一部分
	//	  该函数通过 xdg_surface 接口生成 xdg_toplevel 对象，使其具备窗口管理能力（如标题栏、最小化/最大化、关闭等属性）
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
	// 9. xdg_toplevel_set_title 是 Wayland 协议中 xdg-shell 扩展的核心函数之一，用于为顶级窗口（xdg_toplevel）设置标题栏文本
    xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
	// 10. 提交界面改变
    wl_surface_commit(state.wl_surface);

	// 11. 客户端处理事件循环的核心函数，负责从服务器接收并分派事件
    while (wl_display_dispatch(state.wl_display)) {
        /* This space deliberately left blank */
    }

    return 0;
}
