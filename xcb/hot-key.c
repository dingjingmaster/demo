/*************************************************************************
> FileName: hot-key.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 14 Dec 2023 11:44:37 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xkbcommon/xkbcommon-keysyms.h>


int main() 
{
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;

    // 创建XCB连接
    connection = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(connection)) {
        printf("无法连接到X服务器\n");
        return 1;
    }

    // 获取屏幕信息
    screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    // 创建窗口
    window = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, 1, 1, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);

    xcb_keysym_t*symbols;
    xcb_keycode_t *keycode;

    // 获取键盘符号
    symbols = xcb_key_symbols_alloc(connection);

    // 获取按键的键码
    keycode = xcb_key_symbols_get_keycode(symbols, XKB_KEY_A)[0]; // 这里以按下字母'a'为例

    // 注册快捷键
    xcb_grab_key(connection, 1, window, XCB_MOD_MASK_ANY, *keycode, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);

    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS: {
                xcb_key_press_event_t *key_event = (xcb_key_press_event_t *)event;
                // 处理按键事件
                if (key_event->detail == *keycode) {
                    // 执行相应的操作
                    printf("按下了'a'键\n");
                }
                break;
            }
            default:
                // 其他事件处理
                break;
        }
        free(event);
    }

    xcb_key_symbols_free(symbols);
    xcb_disconnect(connection);

    return 0;
}
