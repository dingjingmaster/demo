/*************************************************************************
> FileName: drop.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 08 Nov 2023 04:34:28 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

int main (int argc, char* argv[])
{
    xcb_connection_t* conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) {
        printf ("无法连接到X服务器\n");
        return 1;
    }

    const xcb_setup_t* setup = xcb_get_setup(conn);
    xcb_screen_t* screen = xcb_setup_roots_iterator(setup).data;

    // 创建窗口
    xcb_window_t win = xcb_generate_id(conn);
    xcb_create_window(conn, XCB_COPY_FROM_PARENT, win, screen->root, 0, 0, 400, 300, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, NULL);

    uint32_t eventMask = XCB_EVENT_MASK_EXPOSURE 
        | XCB_EVENT_MASK_POINTER_MOTION
        | XCB_EVENT_MASK_BUTTON_PRESS
        | XCB_EVENT_MASK_BUTTON_RELEASE
        | XCB_EVENT_MASK_ENTER_WINDOW
        | XCB_EVENT_MASK_LEAVE_WINDOW
        | XCB_EVENT_MASK_KEY_PRESS
        | XCB_EVENT_MASK_KEY_RELEASE
        | XCB_EVENT_MASK_PROPERTY_CHANGE;

    xcb_intern_atom_cookie_t atomCookie = xcb_intern_atom(conn, 1, 12, "XdndAware");
    xcb_intern_atom_reply_t* atomReply = xcb_intern_atom_reply(conn, atomCookie, NULL);
    xcb_atom_t xdndAwareAtom = XCB_ATOM_NONE;
    if (atomReply) {
        xdndAwareAtom = atomReply->atom;
        free (atomReply);
    }

    if (XCB_ATOM_NONE == xdndAwareAtom) {
        printf ("xdnd error\n");
        return 1;
    }

    xcb_change_property(conn, XCB_PROP_MODE_REPLACE, win, xdndAwareAtom, XCB_ATOM_ATOM, 32, 1, &xdndAwareAtom);

    xcb_atom_t versionAtom = xcb_intern_atom_reply(conn, xcb_intern_atom(conn, 0, 11, "XdndProtocolVersion"), NULL)->atom;
    xcb_change_property(conn, XCB_PROP_MODE_REPLACE, win, versionAtom, XCB_ATOM_ATOM, 32, 1, (const uint32_t[]){5});

    xcb_change_window_attributes(conn, win, XCB_CW_EVENT_MASK, &eventMask);

    xcb_map_window(conn, win);
    xcb_flush(conn);

    // 进入事件循环
    xcb_generic_event_t* event;
    while ((event = xcb_wait_for_event(conn))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE: {
                xcb_rectangle_t rect = {0, 0, 400, 400};
                xcb_poly_fill_rectangle(conn, win, screen->black_pixel, 1, &rect);
                xcb_flush(conn);
                break;
            }
            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t* pressEvent = (xcb_button_press_event_t*) event;
                if (pressEvent->detail == XCB_BUTTON_INDEX_1) {
                }
            }
        }
    }

    return 0;
    (void) argc;
    (void) argv;
}
