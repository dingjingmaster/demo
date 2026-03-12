/*************************************************************************
> FileName: get-clipboard.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年03月12日 星期四 17时07分43秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

/**
 * xcb_get_clipboard - 从 X11 剪贴板获取 CLIPBOARD 内容（UTF-8）
 * 返回 malloc 分配的字符串，调用者需 free()
 * 失败返回 NULL
 */
char* xcb_get_clipboard() {
    xcb_connection_t *conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) {
        fprintf(stderr, "无法连接 X server\n");
        return NULL;
    }

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

    // 获取 Atom
    xcb_atom_t clipboard_atom, utf8_atom, property_atom;
    xcb_intern_atom_reply_t *reply_clipboard = xcb_intern_atom_reply(
        conn, xcb_intern_atom(conn, 0, 9, "CLIPBOARD"), NULL);
    xcb_intern_atom_reply_t *reply_utf8 = xcb_intern_atom_reply(
        conn, xcb_intern_atom(conn, 0, 11, "UTF8_STRING"), NULL);
    if (!reply_clipboard || !reply_utf8) {
        free(reply_clipboard); free(reply_utf8);
        xcb_disconnect(conn);
        return NULL;
    }

    clipboard_atom = reply_clipboard->atom;
    utf8_atom = reply_utf8->atom;
    property_atom = XCB_ATOM_PRIMARY; // 临时属性存储
    free(reply_clipboard);
    free(reply_utf8);

    // 创建临时窗口接收事件
    xcb_window_t win = xcb_generate_id(conn);
    uint32_t mask = XCB_CW_EVENT_MASK;
    uint32_t values[] = { XCB_EVENT_MASK_PROPERTY_CHANGE };
    xcb_create_window(conn, XCB_COPY_FROM_PARENT, win,
                      screen->root, 0,0,1,1,0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, mask, values);

    // 请求剪贴板内容
    xcb_convert_selection(conn, win, clipboard_atom, utf8_atom, property_atom, XCB_CURRENT_TIME);
    xcb_flush(conn);

    char *result = NULL;

    // 等待 SelectionNotify
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(conn))) {
        if ((event->response_type & ~0x80) == XCB_SELECTION_NOTIFY) {
            xcb_selection_notify_event_t *sev = (xcb_selection_notify_event_t*)event;
            if (sev->property == XCB_ATOM_NONE) {
                free(event);
                break;
            }

            // 读取属性
            xcb_get_property_cookie_t cookie = xcb_get_property(
                conn, 0, win,
                sev->property,
                XCB_GET_PROPERTY_TYPE_ANY,
                0, 65536); // 最大读取 64KB
            xcb_get_property_reply_t *prop = xcb_get_property_reply(conn, cookie, NULL);
            if (prop) {
                int len = xcb_get_property_value_length(prop);
                if (len > 0) {
                    result = (char*)malloc(len + 1);
                    memcpy(result, xcb_get_property_value(prop), len);
                    result[len] = '\0';
                }
                free(prop);
            }
            free(event);
            break;
        }
        free(event);
    }

    // 清理
    xcb_destroy_window(conn, win);
    xcb_disconnect(conn);

    return result;
}

int main() {
    char *text = xcb_get_clipboard();
    if (text) {
        printf("剪贴板内容: %s\n", text);
        free(text);
    } else {
        printf("剪贴板为空或获取失败\n");
    }
    return 0;
}
