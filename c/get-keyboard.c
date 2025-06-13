/*************************************************************************
> FileName: get-keyboard.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月13日 星期五 11时50分33秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

int is_keyboard_device(const char *device_path) {
    int fd;
    unsigned long evbit[2] = {0};
    unsigned long keybit[KEY_MAX/8 + 1] = {0};
    unsigned long repbit[2] = {0};

    // 打开设备文件
    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    // 获取设备支持的事件类型
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) {
        close(fd);
        return 0;
    }

    // 检查是否支持 EV_KEY 事件
    if (!(evbit[0] & (1 << EV_KEY))) {
        close(fd);
        return 0;
    }

    // 获取支持的按键类型
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0) {
        close(fd);
        return 0;
    }

    // 检查是否支持 EV_REP 事件
    if (ioctl(fd, EVIOCGBIT(EV_REP, sizeof(repbit)), repbit) < 0) {
        close(fd);
        return 0;
    }

    // 检查是否支持常见的键盘按键
    int has_common_keys = 0;
    if (keybit[KEY_A/8] & (1 << (KEY_A % 8))) has_common_keys++;
    if (keybit[KEY_B/8] & (1 << (KEY_B % 8))) has_common_keys++;
    if (keybit[KEY_C/8] & (1 << (KEY_C % 8))) has_common_keys++;
    if (keybit[KEY_ENTER/8] & (1 << (KEY_ENTER % 8))) has_common_keys++;
    if (keybit[KEY_SPACE/8] & (1 << (KEY_SPACE % 8))) has_common_keys++;

    close(fd);

    // 如果支持 EV_KEY 和 EV_REP，并且至少支持 3 个常见按键，则认为是键盘
    return (has_common_keys >= 3 && (repbit[0] & (1 << EV_REP)));
}

int main() {
    int i;
    char device_path[256];
    char name[256] = "Unknown";

    // 遍历所有可能的输入设备
    for (i = 0; i < 32; i++) {
        snprintf(device_path, sizeof(device_path), "/dev/input/event%d", i);

        if (is_keyboard_device(device_path)) {
            // 获取设备名称
            int fd = open(device_path, O_RDONLY);
            if (fd >= 0) {
                if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
                    printf("找到键盘设备: %s (%s)\n", device_path, name);
                }
                close(fd);
            }
        }
    }

    return 0;
}
