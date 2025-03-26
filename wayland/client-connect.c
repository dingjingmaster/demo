/*************************************************************************
> FileName: client-connect.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年03月26日 星期三 08时32分31秒
 ************************************************************************/
#include <stdio.h>
#include <wayland-client.h>

int main(int argc, char *argv[])
{
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display.\n");
        return 1;
    }
    fprintf(stdout, "Connection established!\n");

    wl_display_disconnect(display);

    return 0;
}

