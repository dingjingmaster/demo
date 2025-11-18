/*************************************************************************
> FileName: udev.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月18日 星期二 11时16分42秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>

#define UEVENT_BUFFER_SIZE 2048

int main() {
    int sock = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (sock < 0) { perror("socket"); return 1; }

    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 1; // 接收所有组
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    char buf[UEVENT_BUFFER_SIZE] __attribute__((aligned));

    while (1) {
        int len = recv(sock, buf, sizeof(buf)-1, 0);
        if (len < 0) continue;
        buf[len] = '\0';

        if (strstr(buf, "SUBSYSTEM=block")) {
            printf("Block device event:\n%s\n", buf);
        }
    }

    close(sock);
    return 0;
}
