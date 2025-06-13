/*************************************************************************
> FileName: get-keyboard4.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 11时56分47秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

int is_keyboard_device(const char *device_path) {
    int fd;
    char phys[256] = {0};
    char name[256] = {0};
    
    // 打开设备文件
    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        return 0;
    }
    
    // 获取设备物理路径
    if (ioctl(fd, EVIOCGPHYS(sizeof(phys)), phys) < 0) {
        close(fd);
        return 0;
    }
    
    // 获取设备名称
    if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        close(fd);
        return 0;
    }
    
    close(fd);
    
    // 检查物理路径是否包含键盘特征
    if (strstr(phys, "usb") || strstr(phys, "input")) {
        printf("设备: %s\n", device_path);
        printf("  名称: %s\n", name);
        printf("  物理路径: %s\n", phys);
        return 1;
    }
    
    return 0;
}

int main() {
    int i;
    char device_path[256];
    
    // 遍历所有可能的输入设备
    for (i = 0; i < 32; i++) {
        snprintf(device_path, sizeof(device_path), "/dev/input/event%d", i);
        is_keyboard_device(device_path);
    }
    
    return 0;
}
