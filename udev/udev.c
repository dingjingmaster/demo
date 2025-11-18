/*************************************************************************
> FileName: udev.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月18日 星期二 10时47分34秒
 ************************************************************************/
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    struct udev *udev = udev_new();
    if (!udev) {
        fprintf(stderr, "Cannot create udev context\n");
        return 1;
    }

    // 创建 udev 监控器
    struct udev_monitor *mon = udev_monitor_new_from_netlink(udev, "udev");
    if (!mon) {
        fprintf(stderr, "Cannot create udev monitor\n");
        udev_unref(udev);
        return 1;
    }

    // 只监听 block 子系统
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL);
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);
    printf("Listening for USB block device events...\n");

    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        // 阻塞等待事件
        int ret = select(fd + 1, &fds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(fd, &fds)) {
            struct udev_device *dev = udev_monitor_receive_device(mon);
            if (dev) {
                const char *action = udev_device_get_action(dev);       // add / remove
                const char *devnode = udev_device_get_devnode(dev);     // /dev/sdb1
                const char *bus = udev_device_get_property_value(dev, "ID_BUS");

                // 仅处理 USB 设备
                if (bus && strcmp(bus, "usb") == 0) {
                    printf("USB %s: %s\n", action, devnode);
                }

                udev_device_unref(dev);
            }
        }
    }

    udev_monitor_unref(mon);
    udev_unref(udev);
    return 0;
}
